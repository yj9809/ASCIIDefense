#include "GameLevel.h"
#include "Actor/Bullet.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Goal.h"
#include "Render/Renderer.h"

#include "Core/Input.h"

#include <iostream>
#include <string>
#include <map>
#include <set>

GameLevel::GameLevel()
{
	LoadMap("map.txt");
}

void GameLevel::Tick(float deltaTime)
{
	if(Input::Get().GetKeyDown('P'))
		isDebugPath = !isDebugPath;

	MoveEnemies();

	Vector2 mousePos = Input::Get().GetMousePosition();

	super::Tick(deltaTime);

	SpawnEnemies(deltaTime);

	if (isDebugPath)
	{
		for (Actor* actor : actors)
		{
			if (!actor || !actor->IsActive() || !actor->IsTypeOf<Enemy>()) continue;
			Enemy* e = static_cast<Enemy*>(actor);
			std::vector<Vector2> path = e->GetRemainingPath();
			for (const Vector2& p : path)
			{
				Renderer::Get().Submit("*", p, Color::Gray, 0);
			}
		}
	}

	std::string mouseInfo = "Mouse: (" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")";
	Renderer::Get().Submit(mouseInfo.c_str(), Vector2(mousePos.x, mousePos.y), Color::RedBright, 100);
}

void GameLevel::LoadMap(const char* mapFile)
{
	char path[2048] = {};
	sprintf_s(path, 2048, "../Assets/Maps/%s", mapFile);

	FILE* file = nullptr;
	fopen_s(&file, path, "rt");

	if (!file)
	{
		std::cerr << "GameLevel::LoadMap - Failed to open map file.\n";

		__debugbreak();
	}

	fseek(file, 0, SEEK_END);

	size_t fileSize = ftell(file);

	rewind(file);

	char* buffer = new char[fileSize + 1];

	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	int maxWidth = 0;
	int index = 0;

	Vector2 position;

	std::vector<int> line;

	while (true)
	{
		if (index >= fileSize)
		{
			break;
		}

		char mapCharacter = buffer[index];
		++index;

		if (mapCharacter == '\n')
		{
			maxWidth = max(maxWidth, position.x);
			++position.y;
			position.x = 0;
			SetGrid(line);
			line.clear();
			continue;
		}
		switch (mapCharacter)
		{
		case '#':
			AddNewActor(new Wall(position));
			line.emplace_back(1);
			break;
		case '.':
			Renderer::Get().Submit(" ", position, Color::White, 0);
			line.emplace_back(0);
			break;
		case 'S':
			Renderer::Get().Submit(" ", position, Color::White, 0);
			line.emplace_back(5);
			spawnPoints.emplace_back(position);
			break;
		case 'G':
			AddNewActor(new Goal(position));
			line.emplace_back(6);
			endPoints.emplace_back(position);
			break;
		}
		++position.x;
	}

	spawner = std::make_unique<Spawner>();
	spawner->SetPaths(grid, spawnPoints, endPoints);

	fclose(file);
}

void GameLevel::SetGrid(std::vector<int> line)
{
	grid.push_back(line);
}

void GameLevel::SpawnEnemies(float deltaTime)
{
	if (spawner)
	{
		if (timer > 1.0f && SpawnCount < 10)
		{
			for (const auto& spawnInfo : spawner->GetSpawnInfos())
			{
				Enemy* enemy = new Enemy(spawnInfo.path);
				enemy->SetPosition(spawnInfo.spawnPoint);
				AddNewActor(enemy);
			}
			SpawnCount++;
			timer = 0.0f;
		}
		else
		{
			timer += deltaTime;
		}
	}
}

void GameLevel::MoveEnemies()
{
	using Key = std::pair<int, int>;

	std::map<Key, std::vector<Enemy*>> toGroups;
	std::map<Key, std::vector<Enemy*>> fromMap;
	std::set<Key> willVacate; // 이번 틱에 비워질 from 좌표

	auto toKey = [](const Vector2& p) -> Key { return { p.x, p.y }; };

	// 1) 수집 + 초기화
	for (Actor* actor : actors)
	{
		if (!actor || !actor->IsActive() || !actor->IsTypeOf<Enemy>()) continue;

		Enemy* e = static_cast<Enemy*>(actor);
		e->SetIsMoving(false);

		Key from = toKey(*e->GetPosition());
		Key to = toKey(e->NextPosition());

		fromMap[from].push_back(e);

		if (!e->HasNext() || !e->CanMoveNow())
		{
			continue;
		}

		toGroups[to].push_back(e);
	}	

	// 2) 각 to 그룹 처리
	for (auto& [to, group] : toGroups)
	{
		if (group.empty()) continue;

		// 같은 to면 랜덤 1명 후보
		Enemy* candidate = nullptr;
		if (group.size() == 1)
		{
			candidate = group[0];
		}
		else
		{
			int winner = Util::Random(0, static_cast<int>(group.size() - 1));
			candidate = group[winner];
		}

		Key from = toKey(*candidate->GetPosition());

		// 자기 자리 유지(to==from)는 통과 (원하면 skip 처리)
		if (to == from)
		{
			candidate->SetIsMoving(true);
			//willVacate.insert(from);
			continue;
		}

		// 3) 현재 점유 체크
		auto occIt = fromMap.find(to);
		if (occIt != fromMap.end())
		{
			bool blocked = false;
			for (Enemy* occ : occIt->second)
			{
				Key occFrom = toKey(*occ->GetPosition());
				if (willVacate.find(occFrom) == willVacate.end())
				{
					blocked = true; // 점유자가 안 비우면 진입 불가
					break;
				}
			}
			if (blocked) continue;
		}

		// 4) 이동 허용
		candidate->SetIsMoving(true);
		willVacate.insert(from);
	}
}

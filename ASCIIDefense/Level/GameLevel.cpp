#include "GameLevel.h"
#include "Actor/Bullet.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Goal.h"
#include "Actor/Tower.h"
#include "Render/Renderer.h"

#include "Core/Input.h"
#include "Engine/Engine.h"

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
	if (life <= 0)
	{
		std::cout << "Game Over!\n";
		Engine::Get().QuitEngine();
	}

	currentMousePos = Input::Get().GetMousePosition();

	MoveEnemies();

	super::Tick(deltaTime);

	SpawnEnemies(deltaTime);

	if (Input::Get().GetKeyDown(VK_RBUTTON))
		RemoveTower(currentMousePos);


	RoundButtonClick();

	DebugPath();

	if (Input::Get().GetKeyDown('C'))
	{
		towerCraftMode = (towerCraftMode == Mode::None) ? Mode::Craft : Mode::None;
	}

	if (towerCraftMode == Mode::Craft)
		TowerCrafting(deltaTime);
}

void GameLevel::Draw()
{
	super::Draw();

	DrawUI();

	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[y].size(); ++x)
		{
			if (grid[y][x] == 1)
			{
				Renderer::Get().Submit("#", Vector2(x, y), Color::White, 0);
			}
		}
	}
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
			line.emplace_back(1);
			break;
		case '.':
			line.emplace_back(0);
			break;
		case 'S':
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

Enemy* GameLevel::FindClosestEnemyInRange(const std::function<bool(const Vector2&)>& inRange, const Vector2& center) const
{
	Enemy* bestEnemy = nullptr;
	int bestDistance = INT_MAX;

	for (Actor* actor : actors)
	{
		if (!actor || !actor->IsActive() || !actor->IsTypeOf<Enemy>())
		{
			continue;
		}

		Enemy* enemy = static_cast<Enemy*>(actor);
		const Vector2& enemyPos = *enemy->GetPosition();

		if (!inRange(enemyPos))
		{
			continue;
		}

		int distance = std::abs(enemyPos.x - center.x) + std::abs(enemyPos.y - center.y);
		if (distance < bestDistance)
		{
			bestDistance = distance;
			bestEnemy = enemy;
		}
	}

	return bestEnemy;
}

void GameLevel::SetGrid(std::vector<int> line)
{
	grid.push_back(line);
}

void GameLevel::SpawnEnemies(float deltaTime)
{
	if (spawner && roundActive)
	{
		int spawnLimit = 4 + round;
		if (timer > 1.0f && roundActive && spawnCount < spawnLimit)
		{
			std::vector<Enemy*> newEnemies;
			for (const auto& spawnInfo : spawner->GetSpawnInfos())
			{
				Enemy* enemy = new Enemy(spawnInfo.path, spawnInfo.endPoint);
				enemy->SetPosition(spawnInfo.spawnPoint);
				AddNewActor(enemy);
				newEnemies.push_back(enemy);
				sawnedEnemyCount++;
			}
			spawnCount++;
			timer = 0.0f;
		}
		else if (spawnCount >= spawnLimit && sawnedEnemyCount <= 0)
		{
			roundActive = false;
			spawnCount = 0;
			round++;
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

void GameLevel::DebugPath()
{
	if (Input::Get().GetKeyDown('P'))
		isDebugPath = !isDebugPath;

	if (isDebugPath)
	{
		for (Actor* actor : actors)
		{
			if (!actor || !actor->IsActive() || !actor->IsTypeOf<Enemy>()) continue;
			Enemy* e = static_cast<Enemy*>(actor);
			const std::vector<Vector2>& path = e->GetPath();
			const int startIndex = e->GetCurrentPathIndex();
			for (int i = startIndex; i < static_cast<int>(path.size()); ++i)
			{
				Renderer::Get().Submit("*", path[i], Color::Gray, 0);
			}
		}
	}
}

void GameLevel::TowerCrafting(float deltaTime)
{
	if (Input::Get().GetKeyDown(VK_LBUTTON))
	{
		TryPlaceTower(currentMousePos); // 설치 시도
		previewDirty = true;
	}

	if (currentMousePos != previousMousePos)
	{
		previousMousePos = currentMousePos;
		previewDirty = true;
	}

	previewRecalcTimer += deltaTime;
	if (previewDirty && previewRecalcTimer >= previewRecalcInterval)
	{
		previewColor = CanPlaceTowerAt(currentMousePos) ? Color::Green : Color::RedBright;
		previewDirty = false;
		previewRecalcTimer = 0.0f;
	}

	DrawTowerPreview(currentMousePos);  // 현재 마우스 프리뷰
}

bool GameLevel::CanPreviewTowerAt(const Vector2& center) const
{
	if (grid.empty()) return false;

	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			int x = center.x + dx;
			int y = center.y + dy;

			if (y < 0 || y >= static_cast<int>(grid.size())) return false;
			if (x < 0 || x >= static_cast<int>(grid[y].size())) return false;
			if (grid[y][x] != 0) return false; // 빈칸만 허용
		}
	}
	return true;
}

bool GameLevel::CanPlaceTowerAt(const Vector2& center) const
{
	if (!CanPreviewTowerAt(center))
		return false;

	if (spawnPoints.empty())
		return false;

	int limitY = spawnPoints[0].y - 10;

	if (limitY < center.y + 1)
		return false;

	auto tempGrid = grid;
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx)
			tempGrid[center.y + dy][center.x + dx] = 2;

	Navigation nav;
	std::vector<Vector2> path;

	if (!spawner->SetPaths(tempGrid, spawnPoints, endPoints))
		return false;

	return true;
}

void GameLevel::DrawTowerPreview(const Vector2& center)
{
	if (center.x < 0 || center.y < 0) return;

	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			Vector2 p(center.x + dx, center.y + dy);
			const char* shape = (dx == 0 && dy == 0) ? "T" : "#";
			Renderer::Get().Submit(shape, p, previewColor, 1000);
		}
	}
}

void GameLevel::TryPlaceTower(const Vector2& center)
{
	if (gold < 10)
	{
		return;
	}

	if (!CanPlaceTowerAt(center)) return;

	auto rollback = [&]()
		{
			for (int dy = -1; dy <= 1; ++dy)
				for (int dx = -1; dx <= 1; ++dx)
					grid[center.y + dy][center.x + dx] = 0;
		};

	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx)
			grid[center.y + dy][center.x + dx] = 2;

	if (!RebuildPath(&center))
	{
		rollback();
		return;
	}

	gold -= 10;

	Actor* t = AddNewActorReturn(new Tower(center, towerUpgrade));
	int64_t key = CenterToKey(center);
	towerMap[key] = t;
}

bool GameLevel::RebuildPath(const Vector2* changedCenter)
{
	if (!spawner->SetPaths(grid, spawnPoints, endPoints))
	{
		return false;
	}

	std::vector<std::pair<Enemy*, std::vector<Vector2>>> pending;
	Navigation nav;

	for (Actor* actor : actors)
	{
		if (!actor || !actor->IsActive() || !actor->IsTypeOf<Enemy>()) continue;
		Enemy* e = static_cast<Enemy*>(actor);

		if (changedCenter)
		{
			auto isInsideTowerArea = [changedCenter](const Vector2& p)
				{
					return p.x >= changedCenter->x - 1 && p.x <= changedCenter->x + 1
						&& p.y >= changedCenter->y - 1 && p.y <= changedCenter->y + 1;
				};

			bool affected = isInsideTowerArea(*e->GetPosition());
			if (!affected)
			{
				const std::vector<Vector2>& path = e->GetPath();
				const int startIndex = e->GetCurrentPathIndex();
				for (int i = startIndex; i < static_cast<int>(path.size()); ++i)
				{
					if (isInsideTowerArea(path[i]))
					{
						affected = true;
						break;
					}
				}
			}

			if (!affected)
				continue;
		}

		std::vector<Vector2> newPath;
		if (!nav.FindPath(*e->GetPosition(), e->GetEndPosition(), grid, newPath))
		{
			return false;
		}

		pending.emplace_back(e, std::move(newPath));
	}

	for (auto& [enemy, path] : pending)
	{
		if (enemy && enemy->IsActive())
			enemy->SetPath(std::move(path));
	}

	return true;
}

int64_t GameLevel::Vector2ToKey(const Vector2& center)
{
	return (static_cast<int64_t>(center.x) << 32) | static_cast<uint32_t>(center.y);
}

int64_t GameLevel::CenterToKey(const Vector2& center)
{
	int64_t key = Vector2ToKey(center);

	for (int i = 0; i < 9; i++)
	{
		int x = center.x + (i % 3 - 1);
		int y = center.y + (i / 3 - 1);

		int64_t adjacentKey = Vector2ToKey(Vector2(x, y));
		centerKeyMap[adjacentKey] = center; // 인접한 좌표도 같은 key로 매핑
	}

	return key;
}

void GameLevel::RemoveTower(const Vector2& pos)
{
	int64_t key = Vector2ToKey(pos);

	if (towerMap.find(key) != towerMap.end())
	{
		for (int i = 0; i < 9; i++)
		{
			int x = pos.x + (i % 3 - 1);
			int y = pos.y + (i / 3 - 1);

			grid[y][x] = 0; // 그리드에서 제거
			centerKeyMap.erase(Vector2ToKey(Vector2(x, y))); // centerKeyMap에서 제거
		}

		if (!RebuildPath())
			return;

		towerMap[key]->Destroy();
		towerMap.erase(key);
		gold += 10;
	}

	if (centerKeyMap.find(key) != centerKeyMap.end())
	{
		Vector2 centerVector = centerKeyMap[key];

		for (int i = 0; i < 9; i++)
		{
			int x = centerVector.x + (i % 3 - 1);
			int y = centerVector.y + (i / 3 - 1);

			grid[y][x] = 0; // 그리드에서 제거
			centerKeyMap.erase(Vector2ToKey(Vector2(x, y))); // centerKeyMap에서 제거
		}

		int64_t centerKey = Vector2ToKey(centerVector);

		if (!RebuildPath())
			return;

		towerMap[centerKey]->Destroy();
		towerMap.erase(centerKey);
		gold += 10;
	}
}

void GameLevel::DrawUI()
{
	Color buttonColor = Color::White;
	Color upgradeButtonColor = Color::White;

	if (roundActive)
	{
		buttonColor = Color::Gray;
	}

	if (!roundActive && hover)
	{
		buttonColor = Color::Yellow;
	}
	else if (!roundActive)
	{
		buttonColor = Color::White;
	}

	if (upgradeHover)
	{
		upgradeButtonColor = Color::Yellow;

	}
	else
	{
		upgradeButtonColor = Color::White;
	}

	sprintf_s(lifeText, "Life: %d", life);
	sprintf_s(goldText, "Gold: %d", gold);
	sprintf_s(roundText, "Round: %d", round);
	sprintf_s(spawnText, "Spawned: %d/%d", sawnedEnemyCount, (4 + round) * 10);
	sprintf_s(towerUpgradeText, "Tower Upgrade: %d", towerUpgrade);
	sprintf_s(enemyHpText, "Enemy HP: %d", 10 + ((round - 1) * (5 * (1 + (round / 3)))));
	sprintf_s(towerDamage, "Tower Damage: %d", 5 + (towerUpgrade * 5));

	Renderer::Get().Submit(lifeText, Vector2(30, 0), Color::RedBright, 1000);
	Renderer::Get().Submit(goldText, Vector2(30, 1), Color::Yellow, 1000);
	Renderer::Get().Submit(roundText, Vector2(30, 2), Color::White, 1000);
	Renderer::Get().Submit(spawnText, Vector2(30, 3), Color::White, 1000);
	Renderer::Get().Submit(towerUpgradeText, Vector2(30, 4), Color::White, 1000);

	Renderer::Get().Submit(enemyHpText, Vector2(30, 7), Color::White, 1000);
	Renderer::Get().Submit(towerDamage, Vector2(30, 8), Color::White, 1000);

	Renderer::Get().Submit("[Start Round]", Vector2(30, 15), buttonColor, 1000);
	Renderer::Get().Submit("[Tower Upgrad]", Vector2(30, 16), upgradeButtonColor, 1000);
	Renderer::Get().Submit(
		"Help\nPress C to enter tower placement mode.\nPlacing a tower costs 10 Gold each.\nThe tower upgrade cost is 50 Gold."
		, Vector2(30, 35), Color::Gray, 1000);

}

void GameLevel::RoundButtonClick()
{
	if (currentMousePos.x >= 30 && currentMousePos.x <= 42 && currentMousePos.y == 15 && !roundActive)
	{
		hover = true;
	}
	else
	{
		hover = false;
	}

	if (currentMousePos.x >= 30 && currentMousePos.x <= 43 && currentMousePos.y == 16)
	{
		upgradeHover = true;

	}
	else
	{
		upgradeHover = false;
	}


	if (hover && !roundActive && Input::Get().GetKeyDown(VK_LBUTTON))
	{
		spawnCount = 0;
		roundActive = true;
	}

	if (upgradeHover && Input::Get().GetKeyDown(VK_LBUTTON) && gold >= 50)
	{
		gold -= 50;
		towerUpgrade++;

		for (auto& t : towerMap)
		{
			if (t.second && t.second->IsActive() && t.second->IsTypeOf<Tower>())
			{
				Tower* tower = static_cast<Tower*>(t.second);
				tower->UpgradeAttack();
			}
		}
	}
}


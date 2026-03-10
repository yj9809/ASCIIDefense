#define NOMINMAX

#include "Spawner.h"
#include "Actor/Enemy.h"

#include "Util/Util.h"

Spawner::Spawner()
{
	navigation = new Navigation();
	Util::SetRandomSeed();
}

bool Spawner::SetPaths(const std::vector<std::vector<int>>& grid, const std::vector<Vector2>& spawnPoints, const std::vector<Vector2>& endPoints)
{
	std::vector<SpawnInfo> newSpawnInfos;

	for (const Vector2& spawn : spawnPoints)
	{		
		int ran = Util::Random(0, static_cast<int>(endPoints.size() - 1));

		std::vector<Vector2> path;

		float noise = Util::RandomRange(1.0f, 3.0f);
		if (!navigation->FindPath(spawn, endPoints[ran], grid, path, noise))
			return false;

		newSpawnInfos.emplace_back(SpawnInfo{ spawn, endPoints[ran], path });
	}

	spawnInfos = std::move(newSpawnInfos);
	return true;
}

void Spawner::StartRound(int round)
{
    currentRound = round;
    spawnCount = 0;
    roundActive = true;
    timer = 0.0f;
}

void Spawner::StopRound()
{
    roundActive = false;
    spawnCount = 0;
}

std::vector<Enemy*> Spawner::Update(float deltaTime, int round, int sawnedEnemyCount)
{
    std::vector<Enemy*> result;

    if (!roundActive)
        return result;

    const float spawnInterval = std::max(0.7f, 1.0f - (round - 1) * 0.05f);
    const int spawnLimit = 4 + round;

    if (timer > spawnInterval && spawnCount < spawnLimit)
    {
        for (const auto& spawnInfo : spawnInfos)
        {
            Enemy* enemy = new Enemy(spawnInfo.path, spawnInfo.endPoint);
            enemy->SetPosition(spawnInfo.spawnPoint);
            result.push_back(enemy);
        }
        spawnCount++;
        timer = 0.0f;
    }
    else
    {
        timer += deltaTime;
    }

    return result;
}

bool Spawner::IsRoundFinished(int sawnedEnemyCount) const
{
    return roundActive && spawnCount >= (4 + currentRound) && sawnedEnemyCount <= 0;
}

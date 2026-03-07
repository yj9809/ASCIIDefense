#include "Spawner.h"
#include "Util/Util.h"

Spawner::Spawner()
{
	navigation = new Navigation();
	Util::SetRandomSeed();
}

bool Spawner::SetPaths(const std::vector<std::vector<int>>& grid, std::vector<Vector2> spawnPoints, std::vector<Vector2> endPoints)
{
	std::vector<SpawnInfo> newSpawnInfos;

	for (const Vector2& spawn : spawnPoints)
	{		
		int ran = Util::Random(0, static_cast<int>(endPoints.size() - 1));

		std::vector<Vector2> path;

		if (!navigation->FindPath(spawn, endPoints[ran], grid, path))
			return false;

		newSpawnInfos.emplace_back(SpawnInfo{ spawn, endPoints[ran], path });
	}

	spawnInfos = std::move(newSpawnInfos);
	return true;
}

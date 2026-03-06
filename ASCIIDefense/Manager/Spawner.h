#pragma once

#include "Component/Agent/Navigation.h"

#include <vector>

using namespace Wanted;

class Spawner
{
public:
	struct SpawnInfo
	{
		Vector2 spawnPoint;
		std::vector<Vector2> path;
	};

public:
	Spawner();
		
	// 스폰 지점과 목표 지점 사이의 경로를 계산하여 paths 벡터에 저장하는 함수.
	void SetPaths(const std::vector<std::vector<int>>& grid, std::vector<Vector2> spawnPoints, std::vector<Vector2> endPoints);

	// Getter.
	const std::vector<SpawnInfo>& GetSpawnInfos() const { return spawnInfos; }
private:
	Navigation* navigation = nullptr;

	std::vector<SpawnInfo> spawnInfos;
};


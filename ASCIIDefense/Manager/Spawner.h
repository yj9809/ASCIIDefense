#pragma once

#include "Component/Agent/Navigation.h"

#include <vector>

using namespace Wanted;

class Enemy;

class Spawner
{
public:
	struct SpawnInfo
	{
		Vector2 spawnPoint;
		Vector2 endPoint;
		std::vector<Vector2> path;
	};

public:
	Spawner();
		
	// 스폰 지점과 목표 지점 사이의 경로를 계산하여 paths 벡터에 저장하는 함수.
	bool SetPaths(const std::vector<std::vector<int>>& grid, const std::vector<Vector2>& spawnPoints, const std::vector<Vector2>& endPoints);

	// 라운드 시작.
	void StartRound(int round);

	// 라운드 종료.
	void StopRound();

	// 매 틱 호출 — 생성할 Enemy 목록 반환.
	std::vector<Enemy*> Update(float deltaTime, int round, int sawnedEnemyCount);

	// 라운드 종료 여부.
	bool IsRoundFinished(int sawnedEnemyCount) const;

	// Getter.
	bool IsActive() const { return roundActive; }
	const std::vector<SpawnInfo>& GetSpawnInfos() const { return spawnInfos; }

private:
	Navigation* navigation = nullptr;

	std::vector<SpawnInfo> spawnInfos;

	int currentRound = 1;

	float timer = 0.0f;
	int spawnCount = 0;
	bool roundActive = false;
};


#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Manager/Spawner.h"

#include <vector>
#include <memory>

using namespace Wanted;

class Enemy;

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	virtual void Tick(float deltaTime) override;

	void LoadMap(const char* mapFile);

	std::vector<std::vector<int>> GetGrid() const { return grid; }
private:
	void SetGrid(std::vector<int> line);

	void SpawnEnemies(float deltaTime);

	void MoveEnemies();

private:
	// 맵 데이터를 저장하는 2D 벡터.
	// 0: 이동 가능 공간, 1: 벽, 2: 타워, 5: 스폰 지점, 6: 목표 지점.
	std::vector<std::vector<int>> grid;

	// 스폰 지점 위치를 저장하는 벡터.
	std::vector<Vector2> spawnPoints;

	// 목표 지점 위치를 저장하는 벡터.
	std::vector<Vector2> endPoints;

	// 적 생성 관리 객체.
	std::unique_ptr<Spawner> spawner;

	bool isDebugPath = false;
	int SpawnCount = 0;
	float timer = 0.0f;
};


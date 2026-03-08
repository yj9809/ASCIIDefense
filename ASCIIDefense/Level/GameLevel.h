#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Manager/Spawner.h"

#include <vector>
#include <memory>
#include <functional>

using namespace Wanted;

class Enemy;

class GameLevel : public Level
{
private:
	enum class TowerCraftMode
	{
		None,
		Craft
	};

	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void LoadMap(const char* mapFile);

	Enemy* FindClosestEnemyInRange(const std::function<bool(const Vector2&)>& inRange, const Vector2& center) const;

	std::vector<std::vector<int>> GetGrid() const { return grid; }
private:
	void SetGrid(std::vector<int> line);

	void SpawnEnemies(float deltaTime);

	void MoveEnemies();

	void DebugPath();

	void TowerCrafting(float deltaTime);

	bool CanPreviewTowerAt(const Vector2& center) const;
	void DrawTowerPreview(const Vector2& center);

	void TryPlaceTower(const Vector2& center);

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

	TowerCraftMode towerCraftMode = TowerCraftMode::None;

	bool isDebugPath = false;
	int spawnCount = 10;
	float timer = 0.0f;
};


#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Manager/Spawner.h"

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <queue>

using namespace Wanted;

class Enemy;
class Tower;

class GameLevel : public Level
{
private:

	enum class Mode
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

	inline void SetLife() { life--; }

	inline void AddGold() { gold += 1; }

	inline void EnemyDestroy() { sawnedEnemyCount--; }

	inline int GetRound() const { return round; }
private:
	void SetGrid(std::vector<int> line);

	void SpawnEnemies(float deltaTime);

	void MoveEnemies();

	void DebugPath();

	void TowerCrafting(float deltaTime);

	bool CanPreviewTowerAt(const Vector2& center) const;
	bool CanPlaceTowerAt(const Vector2& center) const;
	void DrawTowerPreview(const Vector2& center);

	void TryPlaceTower(const Vector2& center);
	bool RebuildPath(const Vector2* changedCenter = nullptr);
	void ProcessPathRebuildQueue();

	int64_t Vector2ToKey(const Vector2& center);
	std::pair<int, int> ToMoveKey(const Vector2& p);

	int64_t CenterToKey(const Vector2& center);

	void RemoveTower(const Vector2& pos);

	void DrawUI();

	void RoundButtonClick();

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

	Mode towerCraftMode = Mode::None;

	Color previewColor = Color::RedBright;

	Vector2 currentMousePos;
	Vector2 previousMousePos;
	bool previewDirty = true;

	std::unordered_map<int64_t, Vector2> centerKeyMap;
	std::unordered_map<int64_t, Actor*> towerMap;

	std::queue<Enemy*> pathRebuildQueue;

	bool isDebugPath = false;

	bool hover = false;
	bool upgradeHover = false;

	int life = 10;
	int gold = 100;
	int round = 1;
	int sawnedEnemyCount = 0;
	int towerUpgrade = 0;

	char lifeText[32] = {};
	char goldText[32] = {};
	char roundText[32] = {};
	char spawnText[32] = {};
	char towerUpgradeText[32] = {};
	char enemyHpText[32] = {};
	char towerDamage[32] = {};
};


#define NOMINMAX

#include "Tower.h"
#include "Level/GameLevel.h"
#include "Actor/Enemy.h"
#include "Actor/Bullet.h"

#include <cmath>
#include <iostream>

static std::vector<std::vector<int>> directions = {
	{ -1, -1 }, // 좌상
	{ 0, -1 }, // 상
	{ 1, -1 }, // 우상
	{ -1, 0 }, // 좌
	{ 1, 0 }, // 우
	{ -1, 1 }, // 좌하
	{ 0, 1 }, // 하
	{ 1, 1 } // 우하
};

Tower::Tower(const Vector2& position, int attackUpgrad)
	: Actor("###\n#T#\n###", Vector2(position.x - 1, position.y - 1), Color::Yellow)
{
	sortingOrder = 1;

	centerPosition = position;
	attockUpgrade = attackUpgrad;
}

void Tower::Tick(float deltaTime)
{
	Enemy* e = GetOwner()->As<GameLevel>()->FindClosestEnemyInRange(
		[this](const Vector2& enemyPos) { return InRectRange(enemyPos); },
		centerPosition
	);

	if (timer >= 1.0f && e)
	{
		int idx = DirectionIndex(centerPosition, *e->GetPosition());

		if (idx == -1)
		{
			return;
		}

		GetOwner()->AddNewActor(new Bullet(centerPosition, idx, directions[idx], attockUpgrade));
		timer = 0.0f;
	}
	else 
	{
		timer += deltaTime;
	}
}

bool Tower::InRectRange(const Vector2& enemyPosition) const
{
	return (
		std::abs(enemyPosition.x - centerPosition.x) <= range
		&& std::abs(enemyPosition.y - centerPosition.y) <= range
		);
}

int Tower::DirectionIndex(const Vector2& myPos, const Vector2& targetPos)
{
	int dx = targetPos.x - myPos.x;
	int dy = targetPos.y - myPos.y;

	if (dx == 0 && dy == 0)
		return -1;

	float angle = std::sqrt(float(dx * dx + dy * dy));;
	float x = dx / angle;
	float y = dy / angle;


	float best = 0;
	float maxDot = -std::numeric_limits<float>::min();

	for (int i = 0; i < directions.size(); ++i)
	{
		float vx = float(directions[i][0]);
		float vy = float(directions[i][1]);
		float vlen = std::sqrt(vx * vx + vy * vy); // 축:1, 대각:sqrt(2)
		vx /= vlen;
		vy /= vlen;

		float dot = x * vx + y * vy; // 클수록 방향이 가깝다
		if (dot > maxDot) { maxDot = dot; best = i; }
	}
	return best;
}

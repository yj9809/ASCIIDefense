#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Tower : public Actor
{
	RTTI_DECLARATIONS(Tower, Actor)

public:
	Tower(const Vector2& position);

	virtual void Tick(float deltaTime) override;

	bool InRectRange(const Vector2& enemyPosition) const;

	int DirectionIndex(const Vector2& myPos, const Vector2& targetPos);

private:
	Vector2 centerPosition;

	int range = 3;

	float timer = 0.0f;
};


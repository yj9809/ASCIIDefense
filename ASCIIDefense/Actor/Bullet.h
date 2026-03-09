#pragma once

#include "Actor/Actor.h"

class Bullet : public Actor
{
	RTTI_DECLARATIONS(Bullet, Actor)

public:
	Bullet(const Vector2& towerPos, int dirIndex, std::vector<int> dir, int attakUpgrad);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	virtual void OnCollision(const CollisionEvent& e, Actor* other) override;

private:
	float moveSpeed = 12.0f;

	float firstX = 0.0f;
	float firstY = 0.0f;

	float xPosition = 0.0f;
	float yPosition = 0.0f;

	std::vector<int> dir;

	int damage = 5;
};


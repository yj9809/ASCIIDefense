#pragma once

#include "Actor/Actor.h"

class Bullet : public Actor
{
	RTTI_DECLARATIONS(Bullet, Actor)

public:
	Bullet(const Vector2& towerPos, int dirIndex, std::vector<int> dir);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	virtual void OnCollision(const CollisionEvent& e, Actor* other) override;

private:
	float moveSpeed = 40.0f;

	float xPosition = 0.0f;
	float yPosition = 0.0f;

	std::vector<int> dir;
};


#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Goal : public Actor
{
	RTTI_DECLARATIONS(Goal, Actor)

public:
	Goal(const Vector2& position);

	virtual void BeginPlay() override;

	virtual void OnCollision(const CollisionEvent& e, Actor* other) override;
};


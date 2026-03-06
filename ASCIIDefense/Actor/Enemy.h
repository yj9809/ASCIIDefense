#pragma once

#include "Actor/Actor.h"
#include "Component/Agent/Agent.h"

using namespace Wanted;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)
public:
	Enemy(std::vector<Vector2> path);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	Vector2 NextPosition() const;
	
	bool HasNext() const;
	
	bool CanMoveNow() const;

	inline void SetIsMoving(bool moving) 
	{ 
		if (agent)
			agent->SetIsMoving(moving);
	}

	// Getter.
	std::vector<Vector2> GetRemainingPath() const;

private:
	Agent* agent = nullptr;

	std::vector<Vector2> path;

	float timer = 0.0f;
};

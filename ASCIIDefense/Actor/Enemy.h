#pragma once

#include "Actor/Actor.h"
#include "Component/Agent/Agent.h"

using namespace Wanted;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)
public:
	Enemy(std::vector<Vector2> path, Vector2 endPosition);

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

	inline void SetPath(std::vector<Vector2> newPath)
	{
		if (agent)
			agent->SetPath(std::move(newPath));
	}

	// Getter.
	std::vector<Vector2> GetRemainingPath() const;

	inline const Vector2 GetEndPosition() const { return endPosition; }

private:
	Agent* agent = nullptr;

	std::vector<Vector2> path;

	Vector2 endPosition;

	float timer = 0.0f;
};

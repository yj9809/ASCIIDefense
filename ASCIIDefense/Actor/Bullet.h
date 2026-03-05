#pragma once

#include "Actor/Actor.h"

namespace Wanted
{
	class Bullet : public Actor
	{
		RTTI_DECLARATIONS(Bullet, Actor)

	public:
		Bullet();

		virtual void BeginPlay() override;
		virtual void Tick(float deltaTime) override;

		virtual void OnCollision(const CollisionEvent& e, Actor* other) override;

	private:
		float moveSpeed = 20.0f;

		float xPosition = 0.0f;
	};
}


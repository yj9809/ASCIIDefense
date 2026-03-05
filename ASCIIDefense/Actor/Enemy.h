#pragma once

#include "Actor/Actor.h"

namespace Wanted
{
	class Enemy : public Actor
	{
		RTTI_DECLARATIONS(Enemy, Actor)
	public:
		Enemy();

		virtual void BeginPlay() override;
		virtual void Tick(float deltaTime) override;

	private:

	};
}


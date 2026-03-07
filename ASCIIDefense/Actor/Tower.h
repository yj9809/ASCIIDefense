#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Tower : public Actor
{
	RTTI_DECLARATIONS(Tower, Actor)

public:
	Tower(const Vector2& position);
};


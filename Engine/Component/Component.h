#pragma once

#include "Common/RTTI.h"
#include "Component/Collision/Util/CollisionType.h"

namespace Wanted
{
	class Actor;

	class Component : public RTTI
	{
		RTTI_DECLARATIONS(Component, RTTI)

	public:
		explicit Component(Actor* owner) : owner(owner) {}
		virtual ~Component() = default;

		virtual void OnAdd() = 0;
		virtual void OnRemove() = 0;

		CollisionID GetId() const { return id; }
	protected:
		Actor* owner = nullptr;

		CollisionID id = 0;
	};
}

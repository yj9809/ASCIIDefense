#include "Actor/Actor.h"
#include "Level/Level.h"
#include "BoxCollider2D.h"
#include "Component/Collision/System/CollisionSystem.h"

namespace Wanted
{
	BoxCollider2D::BoxCollider2D(Actor* owner, CollisionLayer layer, LayerMask mask, const ColliderBox& box)
		: Component(owner), layer(layer), mask(mask), box(box)
	{
	}

	BoxCollider2D::~BoxCollider2D()
	{
		OnRemove();
	}

	void BoxCollider2D::OnAdd()
	{
		id = owner->GetOwner()->GetCollisionSystem()->Register(*this);
	}

	void BoxCollider2D::OnRemove()
	{
		owner->GetOwner()->GetCollisionSystem()->Unregister(id);
		owner->GetOwner()->UnregisterActor(id);
		id = 0;
	}

	const Vector2* BoxCollider2D::GetOwnerPosition() const
	{
		return owner->GetPosition();
	}
}
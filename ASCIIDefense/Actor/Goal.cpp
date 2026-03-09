#include "Goal.h"
#include "Enemy.h"

#include "Level/GameLevel.h"

Goal::Goal(const Vector2& position)
	: super("G", position, Color::Green)
{

}

void Goal::BeginPlay()
{
	super::BeginPlay();
	BoxCollider2D::ColliderBox box{ width, height };
	AddComponent<BoxCollider2D>(CollisionLayer::Goal, GetLayerMask(CollisionLayer::Enemy), box);
}

void Goal::OnCollision(const CollisionEvent& e, Actor* other)
{
	if (!IsActive() || !other->IsActive())
		return;

	if (other->IsTypeOf<Enemy>())
	{
		GetOwner()->As<GameLevel>()->SetLife();
		GetOwner()->As<GameLevel>()->EnemyDestroy();
		other->Destroy();
	}
}

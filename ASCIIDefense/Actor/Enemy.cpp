#include "Enemy.h"
#include "Component/Collision/Collider/BoxCollider2D.h"
#include "Component/Collision/Util/CollisionType.h"

Wanted::Enemy::Enemy()
	: Actor("E", Vector2::Zero, Color::Red)
{
	
}

void Wanted::Enemy::BeginPlay()
{
	BoxCollider2D::ColliderBox box{ width, height };
	AddComponent<BoxCollider2D>(CollisionLayer::Enemy, GetLayerMask(CollisionLayer::Bullet), box);
}

void Wanted::Enemy::Tick(float deltaTime)
{
}

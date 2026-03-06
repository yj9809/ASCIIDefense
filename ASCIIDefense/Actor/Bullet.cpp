#include "Bullet.h"
#include "Actor/Enemy.h"
#include "Component/Collision/Collider/BoxCollider2D.h"
#include "Component/Collision/Util/CollisionType.h"


Bullet::Bullet()
	: Actor("-", Vector2(0, 5), Color::White)
{

}

void Bullet::BeginPlay()
{
	super::BeginPlay();

	BoxCollider2D::ColliderBox box{ width, height };
	AddComponent<BoxCollider2D>(CollisionLayer::Bullet, GetLayerMask(CollisionLayer::Enemy), box);
}

void Bullet::Tick(float deltaTime)
{
	xPosition += moveSpeed * deltaTime;

	SetPosition(Vector2(static_cast<int>(xPosition), position.y));
}

void Bullet::OnCollision(const CollisionEvent& e, Actor* other)
{
	if (DestroyRequested()) 
		return;

	if (!IsActive() || !other->IsActive()) 
		return;

	if (other->IsTypeOf<Enemy>())
	{
		// 충돌한 액터가 Enemy인 경우, Bullet과 Enemy 모두 제거 요청.
		this->Destroy();
		other->Destroy();
	}
}

#include "Bullet.h"
#include "Actor/Enemy.h"
#include "Component/Collision/Collider/BoxCollider2D.h"
#include "Component/Collision/Util/CollisionType.h"

namespace Wanted
{
	Bullet::Bullet()
		: Actor("-", Vector2(0, 5), Color::White)
	{

	}

	void Bullet::BeginPlay()
	{
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
		if (other->IsTypeOf<Enemy>())
		{
			// 충돌한 액터가 Enemy인 경우, Bullet과 Enemy 모두 제거 요청.
			this->Destroy();
			other->Destroy();
		}
	}
}

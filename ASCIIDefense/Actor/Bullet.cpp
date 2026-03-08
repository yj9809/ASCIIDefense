#define NOMINMAX

#include "Bullet.h"
#include "Actor/Enemy.h"
#include "Component/Collision/Collider/BoxCollider2D.h"
#include "Component/Collision/Util/CollisionType.h"

#include <limits>

static std::vector<char> directionImages = {
	'\\',
	'|',
	'/',
	'-',
	'-',
	'/',
	'|',
	'\\'
};

Bullet::Bullet(const Vector2& towerPos, int index, std::vector<int> dir)
	: Actor("-", towerPos, Color::White)
{
	sortingOrder = 99;

	xPosition = static_cast<float>(towerPos.x);
	yPosition = static_cast<float>(towerPos.y);

	this->dir = dir;
	image[0] = directionImages[index];
}

void Bullet::BeginPlay()
{
	super::BeginPlay();

	BoxCollider2D::ColliderBox box{ width, height };
	AddComponent<BoxCollider2D>(CollisionLayer::Bullet, GetLayerMask(CollisionLayer::Enemy), box);
}

void Bullet::Tick(float deltaTime)
{
	xPosition += moveSpeed * deltaTime * dir[0];
	yPosition += moveSpeed * deltaTime * dir[1];

	SetPosition(Vector2(std::round(xPosition), std::round(yPosition)));
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

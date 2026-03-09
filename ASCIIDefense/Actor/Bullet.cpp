#define NOMINMAX

#include "Bullet.h"
#include "Actor/Enemy.h"
#include "Level/GameLevel.h"
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

Bullet::Bullet(const Vector2& towerPos, int index, std::vector<int> dir, int attakUpgrad)
	: Actor("-", towerPos, Color::White)
{
	sortingOrder = 99;

	xPosition = static_cast<float>(towerPos.x);
	yPosition = static_cast<float>(towerPos.y);
	
	firstX = xPosition;
	firstY = yPosition;

	damage = 5 + (attakUpgrad * 5);

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
	int y = GetOwner()->As<GameLevel>()->GetGrid().size();
	int x = GetOwner()->As<GameLevel>()->GetGrid()[0].size();

	if(4 > position.x || x - 3 <= position.x || 0 > position.y || y <= position.y)
	{
		Destroy();
		return;
	}

	if (std::abs(firstX - xPosition) > 4 || std::abs(firstY - yPosition) > 4)
	{
		Destroy();
		return;
	}

 	float dx = static_cast<float>(dir[0]);
	float dy = static_cast<float>(dir[1]);
	float len = std::sqrt(dx * dx + dy * dy);

	xPosition += moveSpeed * deltaTime * (dx / len);
	yPosition += moveSpeed * deltaTime * (dy / len);

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
		other->As<Enemy>()->SetHp(damage);		
		Destroy();
	}
}

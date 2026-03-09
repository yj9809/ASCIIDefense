#include "Enemy.h"
#include "Level/GameLevel.h"

#include "Component/Collision/Collider/BoxCollider2D.h"
#include "Component/Collision/Util/CollisionType.h"
#include "Util/Util.h"

Enemy::Enemy(std::vector<Vector2> path, Vector2 endPosition)
	: Actor("E", Vector2::Zero, Color::Red), path(std::move(path)), endPosition(endPosition)
{
	
}

void Enemy::BeginPlay()
{
	super::BeginPlay();

	BoxCollider2D::ColliderBox box{ width, height };
	AddComponent<BoxCollider2D>(CollisionLayer::Enemy, GetLayerMask(CollisionLayer::Bullet) | GetLayerMask(CollisionLayer::Goal), box);

	agent = AddComponent<Agent>(std::move(path));
	int round = GetOwner()->As<GameLevel>()->GetRound();
	hp = 10 + ((round - 1) * (5 * (1 + (round / 3))));
}

void Enemy::Tick(float deltaTime)
{
	if (!IsActive())
	{
		return;
	}

	if(hp <= 0)
	{
		GetOwner()->As<GameLevel>()->AddGold();
		GetOwner()->As<GameLevel>()->EnemyDestroy();
		Destroy();
		return;
	}

	super::Tick(deltaTime);

	if (agent)
	{
		if (CanMoveNow())
		{
			agent->Move();
			timer = 0.0f;
		}
		else
		{
			timer += deltaTime;
		}
	}
}

Vector2 Enemy::NextPosition() const
{
	if (agent)
	{
		return agent->NextPosition();
	}
	return Vector2();
}

bool Enemy::HasNext() const
{
	if (agent)
	{
		return agent->HasNext();
	}
	return false;
}

bool Enemy::CanMoveNow() const
{
	return timer > 0.3f;
}

std::vector<Vector2> Enemy::GetRemainingPath() const
{
	return agent ? agent->GetRemainingPath() : std::vector<Vector2>();
}

const std::vector<Vector2>& Enemy::GetPath() const
{
	static const std::vector<Vector2> emptyPath;
	return agent ? agent->GetPath() : emptyPath;
}

int Enemy::GetCurrentPathIndex() const
{
	return agent ? agent->GetCurrentPathIndex() : 0;
}

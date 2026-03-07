#include "Level.h"
#include "Actor/Actor.h"
#include "Component/Collision/System/CollisionSystem.h"

namespace Wanted
{
	Level::Level()
	{
		// 충돌 시스템 생성.
		collisionSystem = std::make_unique<CollisionSystem>();
	}

	Level::~Level()
	{
		// �޸� ����.
		for (Actor*& actor : actors)
		{
			// ���� ��ü �޸� ����.
			if (actor)
			{
				delete actor;
				actor = nullptr;
			}
		}

		// �迭 �ʱ�ȭ.
		actors.clear();

		for (Actor*& actor : addRequestedActors)
		{
			// ���� ��ü �޸� ����.
			if (actor)
			{
				delete actor;
				actor = nullptr;
			}
		}
		addRequestedActors.clear();
	}

	void Level::BeginPlay()
	{
		// ���Ϳ� �̺�Ʈ �긮��.
		for (Actor* actor : actors)
		{
			// �̹� BeginPlay ȣ��� ���ʹ� �ǳʶٱ�.
			if (actor->HasBeganPlay())
			{
				continue;
			}

			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		// ���Ϳ� �̺�Ʈ �긮��.
		for (Actor* actor : actors)
		{
			if (!actor->IsActive())
			{
				continue;
			}

			actor->Tick(deltaTime);
		}

		// 충돌 시스템 업데이트.
		collisionSystem->Tick();

		if (collisionSystem->events.size() > 0)
		{
			for (const CollisionEvent& e : collisionSystem->events)
			{
				Actor* self = FindActorByCollisionID(e.selfID);   // Bullet
				Actor* other = FindActorByCollisionID(e.otherID); // Enemy
				if (!self || !other) 
					continue;

				if (!self->IsActive() || !other->IsActive()) 
					continue;

				self->OnCollision(e, other);
				other->OnCollision({ e.type, e.otherID, e.selfID }, self);
			}
			collisionSystem->events.clear();
		}
	}

	void Level::Draw()
	{
		//// ���Ϳ� �̺�Ʈ �긮��.
		//for (Actor* actor : actors)
		//{
		//	// ���� ��ġ�� �ٸ� ���Ͱ� �ִ��� Ȯ���ϱ� ����.
		//	Actor* search = nullptr;
		//	for (Actor* otherActor : actors)
		//	{
		//		// ���� ���ʹ� �� ����.
		//		if (actor == otherActor)
		//		{
		//			continue;
		//		}

		//		// ��ġ ��.
		//		if (actor->GetPosition() == otherActor->GetPosition())
		//		{
		//			// ���� ���� ��.
		//			if (actor->GetSortingOrder() < otherActor->GetSortingOrder())
		//			{
		//				search = otherActor;
		//				break;
		//			}
		//		}
		//	}

		//	// ���� ��ġ�� �켱���� ���� �ٸ� ���Ͱ� ������ �ȱ׸�.
		//	if (search)
		//	{
		//		continue;
		//	}

		//	// �׸���.
		//	actor->Draw();
		//}

		// ���� ��ȸ�ϸ鼭 Draw �Լ� ȣ��.
		for (Actor* const actor : actors)
		{
			if (!actor->IsActive())
			{
				continue;
			}

			actor->Draw();
		}
	}

	void Level::AddNewActor(Actor* newActor)
	{
		// ���߿� �߰��� ���� �ӽ� �迭�� ����.
		addRequestedActors.emplace_back(newActor);

		// ���ʽ� ����.
		newActor->SetOwner(this);
	}

	Actor* Level::AddNewActorReturn(Actor* newActor)
	{
		// ���߿� �߰��� ���� �ӽ� �迭�� ����.
		addRequestedActors.emplace_back(newActor);

		// ���ʽ� ����.
		newActor->SetOwner(this);

		return newActor;
	}

	void Level::ProcessAddAndDestroyActors()
	{
		// ���� ó��.
		for (int ix = 0; ix < static_cast<int>(actors.size()); )
		{
			// ���� ��û�� ���Ͱ� �ִ��� Ȯ��.
			if (actors[ix]->DestroyRequested())
			{
				// ���� ó��.
				delete actors[ix];
				if (actors.size() <= 0) {
					continue;
				}
				actors.erase(actors.begin() + ix);
				continue;
			}

			++ix;
		}

		// �߰� ó��.
		if (addRequestedActors.size() == 0)
		{
			return;
		}

		for (Actor* const actor : addRequestedActors)
		{
			actors.emplace_back(actor);
		}

		// ó���� �������� �迭 �ʱ�ȭ.
		addRequestedActors.clear();
	}

	Actor* Level::FindActorByCollisionID(CollisionID id) const
	{
		if (collisionActorMap.find(id) != collisionActorMap.end())
		{
			return collisionActorMap.at(id);
		}

		return nullptr;
	}

	void Level::RegisterActor(CollisionID id, Actor* actor)
	{
		collisionActorMap[id] = actor;
	}

	void Level::UnregisterActor(CollisionID id)
	{
		collisionActorMap.erase(id);
	}
}
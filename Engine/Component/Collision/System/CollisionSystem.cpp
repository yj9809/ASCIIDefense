#include "CollisionSystem.h"

namespace Wanted
{
	CollisionSystem::CollisionSystem()
	{
	}

	CollisionSystem::~CollisionSystem()
	{
	}

	CollisionID CollisionSystem::Register(BoxCollider2D& c)
	{
		if (c.GetId() != 0)
		{
			return c.GetId();
		}

		CollisionID newId;

		if (freeId.empty())
		{
			newId = nextId++;
		}
		else
		{
			newId = freeId.top();
			freeId.pop();
		}

		ColliderInfo info = {
			newId,
			c.GetOwnerPosition(),
			c.GetLayer(),
			c.GetMask(),
			c.GetBox()
		};

		colliders.emplace_back(info);

		return newId;
	}

	void CollisionSystem::Unregister(CollisionID id)
	{
		for (auto it = colliders.begin(); it != colliders.end(); ++it)
		{
			if (it->id == id)
			{
				freeId.push(it->id);
				colliders.erase(it);
				return;
			}
		}
	}
	void CollisionSystem::Tick()
	{
		// 충돌 체크 로직.
		const int n = static_cast<int>(colliders.size());
		for (int i = 0; i < n; ++i)
		{
			for (int j = i + 1; j < n; ++j) // 자기 자신/중복 쌍 방지
			{
				const auto& a = colliders[i];
				const auto& b = colliders[j];

				if (!a.isEnabled || !b.isEnabled)
					continue;

				if (CheckCollision(a, b) && CheckMask(a, b))
				{
					events.push_back({ EventType::Stay, b.id, a.id });
				}
			}
		}
	}

	bool CollisionSystem::CheckCollision(const ColliderInfo& a, const ColliderInfo& b)
	{
		if (a.position->x + (a.box.width - 1) < b.position->x)
			return false;
		if (a.position->x > b.position->x + (b.box.width - 1))
			return false;
		if (a.position->y + (a.box.height - 1) < b.position->y)
			return false;
		if (a.position->y > b.position->y + (b.box.height - 1))
			return false;

		return true;
	}

	bool CollisionSystem::CheckMask(const ColliderInfo& a, const ColliderInfo& b)
	{
		return a.mask & GetLayerMask(b.layer) || b.mask & GetLayerMask(a.layer);
	}

}

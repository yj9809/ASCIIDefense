#pragma once

#include "../Collider/BoxCollider2D.h"
#include "../Util/CollisionType.h"

#include <memory>
#include <vector>
#include <stack>

namespace Wanted
{
	class CollisionSystem
	{
		friend class Level;

	private:
		struct ColliderInfo
		{
			CollisionID id;

			const Vector2* position;

			CollisionLayer layer;
			LayerMask mask;
			BoxCollider2D::ColliderBox box;
			bool isEnabled = true;
		};

	public:
		CollisionSystem();
		~CollisionSystem();

		CollisionID Register(BoxCollider2D& c);
		void Unregister(CollisionID id);

		void Tick();

	private:
		bool CheckCollision(const ColliderInfo& a, const ColliderInfo& b);

		bool CheckMask(const ColliderInfo& a, const ColliderInfo& b);

	private:
		// 콜리더 배열.
		std::vector<ColliderInfo> colliders;

		// 지운 아이디 재할당용.
		std::stack<CollisionID> freeId;

		// 이벤트 큐.
		std::vector<CollisionEvent> events;

		// id 할당용 카운터.
		int nextId = 1;
	};
}


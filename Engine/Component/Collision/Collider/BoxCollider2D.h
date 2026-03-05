#pragma once

#include "Math/Vector2.h"
#include "Component/Component.h"
#include "Component/Collision/Util/CollisionType.h"

#include <memory>

namespace Wanted
{
	class Actor;

	class WANTED_API BoxCollider2D : public Component
	{
		RTTI_DECLARATIONS(BoxCollider2D, Component)

	public:
		struct ColliderBox
		{
			int width;
			int height;
		};

	public:
		BoxCollider2D(
			Actor* owner,
			CollisionLayer layer,
			LayerMask mask,
			const ColliderBox& box
		);
		~BoxCollider2D();

		virtual void OnAdd() override;
		virtual void OnRemove() override;

		// Getter.
		inline CollisionLayer GetLayer() const { return layer; }
		inline LayerMask GetMask() const { return mask; }
		inline ColliderBox GetBox() const { return box; }

		// 오너 포지션 가져오기.
		const Vector2* GetOwnerPosition() const;

	private:
		// 자신의 레이어.
		CollisionLayer layer = CollisionLayer::None;

		// 충돌할 레이어.
		LayerMask mask = 0;

		// 충돌 박스.
		ColliderBox box;
	};
}


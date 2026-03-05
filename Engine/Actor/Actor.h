#pragma once

#include "Common/RTTI.h"
#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Component/Component.h"
#include "Component/Collision/Collider/BoxCollider2D.h"
#include "Component/Agent/Agent.h"

#include <vector>
#include <memory>

namespace Wanted
{
	//class Level;

	class WANTED_API Actor : public RTTI
	{
		RTTI_DECLARATIONS(Actor, RTTI)

	public:
		Actor(
			const char* image = " ",
			const Vector2& position = Vector2::Zero,
			Color color = Color::White
		);
		virtual ~Actor();

		Actor(const Actor&) = delete;
		Actor& operator=(const Actor&) = delete;
		Actor(Actor&&) = default;
		Actor& operator=(Actor&&) = default;

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		void Destroy();

		virtual void OnDestroy();

		void QuitGame();

		bool TestIntersect(const Actor* const other);

		void SetPosition(const Vector2& newPosition);
		inline const Vector2* GetPosition() const { return &position; }

		inline int GetHeight() const { return height; }

		inline void SetOwner(Level* newOwner) { owner = newOwner; }
		inline Level* GetOwner() const { return owner; }

		// Getter.
		inline bool HasBeganPlay() const
		{
			return hasBeganPlay;
		}

		inline bool IsActive() const
		{
			return isActive && !destroyRequested;
		}

		inline bool DestroyRequested() const
		{
			return destroyRequested;
		}

		inline int GetSortingOrder() const { return sortingOrder; }

		inline int GetWidth() const { return width; }

		// 충돌 이벤트 핸들러.
		virtual void OnCollision(const CollisionEvent& e, Actor* other) {}

		// 컴포넌트 추가 함수.
		template <typename T, typename... Ts>
		T* AddComponent(Ts&&... component)
		{
			auto componentPtr = std::make_unique<T>(this, std::forward<Ts>(component)...);
			T* raw = componentPtr.get();

			components.push_back(std::move(componentPtr));

			raw->OnAdd();

			if constexpr (std::is_same_v<T, BoxCollider2D>)
				GetOwner()->RegisterActor(raw->GetId(), this);

			return raw;
		}

	protected:
		bool hasBeganPlay = false;

		bool isActive = true;

		bool destroyRequested = false;

		char* image = nullptr;

		int width = 0;

		int height = 1;

		Color color = Color::White;

		Level* owner = nullptr;

		int sortingOrder = 0;

		Vector2 position;

		std::vector<std::unique_ptr<Component>> components;
	};
}


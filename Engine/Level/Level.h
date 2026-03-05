#pragma once

#include "Common/RTTI.h"
#include "Component/Collision/Util/CollisionType.h"

#include <vector>
#include <memory>
#include <unordered_map>

namespace Wanted
{
	class CollisionSystem;
	class Actor;

	class WANTED_API Level : public RTTI
	{
		RTTI_DECLARATIONS(Level, RTTI)

	public:
		Level();
		virtual ~Level();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		void AddNewActor(Actor* newActor);
		Actor* AddNewActorReturn(Actor* newActor);

		void ProcessAddAndDestroyActors();

		// Collision ID를 통한 액터 검색 함수.
		Actor* FindActorByCollisionID(CollisionID id) const;

		// Collision ID를 통한 액터 검색을 위한 등록/해제 함수.
		void RegisterActor(CollisionID id, Actor* actor);
		void UnregisterActor(CollisionID id);

		// CollisionSystem Getter.
		inline CollisionSystem* GetCollisionSystem() const 
		{
			return collisionSystem.get(); 
		}

	protected:
		std::vector<Actor*> actors;
		std::vector<std::unique_ptr<Actor>> actors1;

		std::vector<Actor*> addRequestedActors;

		std::unique_ptr<CollisionSystem> collisionSystem;

		std::unordered_map<CollisionID, Actor*> collisionActorMap;
	};
}
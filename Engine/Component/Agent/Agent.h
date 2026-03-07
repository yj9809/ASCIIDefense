#pragma once

#include "Math/Vector2.h"
#include "Node.h"
#include "Component/Component.h"

#include <memory>
#include <vector>

namespace Wanted
{
	class Actor;

	class WANTED_API Agent : public Component
	{
	public:
		Agent(Actor* owner, std::vector<Vector2> path);

		virtual void OnAdd() override {}
		virtual void OnRemove() override {}
	public:
		void Move();

		Vector2 NextPosition() const;

		bool HasNext() const;

		inline void SetIsMoving(bool moving) { isMoving = moving; }

		// Getter.
		inline std::vector<Vector2> GetRemainingPath() const
		{
			if (currentPathIndex < path.size())
			{
				return std::vector<Vector2>(path.begin() + currentPathIndex, path.end());
			}
			return {};
		}

		// Setter.
		inline void SetPath(std::vector<Vector2> newPath)
		{
			path = std::move(newPath);
			currentPathIndex = 0;
			isMoving = false;
		}

	private:
		std::vector<Vector2> path;

		int currentPathIndex = 0;

		bool isMoving = false;
	};
}

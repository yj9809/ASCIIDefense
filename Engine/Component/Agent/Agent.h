#pragma once

#include "Math/Vector2.h"
#include "Util/Util.h"
#include "Node.h"
#include "Component/Component.h"

#include <queue>
#include <vector>

namespace Wanted
{
	class WANTED_API Agent : public Component
	{
	private:
		struct Direction
		{
			Vector2 offset;
			float cost;
		};

	public:
		~Agent();

	public:
		bool FindPath(
			const Vector2& start,
			const Vector2& target,
			const std::vector<std::vector<int>>& grid,
			std::vector<Vector2>& outPath
		);

		bool IsGridRange(const Vector2& position, const std::vector<std::vector<int>>& grid) const;

		float GetHCost(const Vector2& position, const Vector2& target) const;

		bool IsCloseToTarget(const Vector2& position, float cost) const;

		std::vector<Vector2> ConstructPath(Node* target);

	private:
		void ClearLists();

	private:
		std::vector<Node*> openList;

		std::vector<Node*> closedList;

		Node* startNode = nullptr;
	};
}


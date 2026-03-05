#pragma once

#include "Math/Vector2.h"
#include "Component/Component.h"

#include <vector>

namespace Wanted
{
	class WANTED_API Agent : public Component
	{
	public:
		bool FindPath(
			const Vector2& start,
			const Vector2& target,
			std::vector<std::vector<int>>& grid,
			std::vector<Vector2>& outPath
		);

		std::vector<Vector2> ConstructPath(const Vector2& target);

	private:
		std::vector<Vector2> openList;

		std::vector<Vector2> closedList;

		Vector2 startPosition;

		Vector2 targetPosition;
	};
}


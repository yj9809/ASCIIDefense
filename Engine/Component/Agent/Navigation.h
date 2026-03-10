#pragma once

#include "Common/Common.h"
#include "Math/Vector2.h"

#include <vector>

namespace Wanted
{
	class WANTED_API Navigation
	{
	private:
		struct Direction
		{
			Vector2 offset;
			float cost;
		};

	public:
		bool FindPath(
			const Vector2& start,
			const Vector2& target,
			const std::vector<std::vector<int>>& grid,
			std::vector<Vector2>& outPath,
			float hNoise = 0.0f
		);

		bool IsGridRange(const Vector2& position, const std::vector<std::vector<int>>& grid) const;

		float GetHCost(const Vector2& position, const Vector2& target, float noise) const;
	};
}


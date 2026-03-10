#define NOMINMAX
#include "Navigation.h"
#include "Util/Util.h"

#include <cmath>
#include <algorithm>
#include <queue>
#include <limits>
#define DIAGONAL_COST 1.41421356f

namespace Wanted
{
	bool Navigation::FindPath(
		const Vector2& start, 
		const Vector2& target, 
		const std::vector<std::vector<int>>& grid, 
		std::vector<Vector2>& outPath,
		float hNoise
	)
	{
		outPath.clear();

		if (IsGridRange(start, grid) || IsGridRange(target, grid))
		{
			return false;
		}		

		const int height = static_cast<int>(grid.size());
		const int width = static_cast<int>(grid[0].size());
		const float kInf = std::numeric_limits<float>::infinity();

		std::vector<std::vector<float>> cellNoise(height, std::vector<float>(width, 0.0f));
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
				cellNoise[y][x] = Util::RandomRange(0.0f, hNoise);

		struct OpenEntry
		{
			float fCost;
			float gCost;
			Vector2 position;
		};
		struct OpenEntryCompare
		{
			bool operator()(const OpenEntry& a, const OpenEntry& b) const
			{
				if (a.fCost == b.fCost)
				{
					return a.gCost > b.gCost;
				}
				return a.fCost > b.fCost;
			}
		};

		std::priority_queue<OpenEntry, std::vector<OpenEntry>, OpenEntryCompare> openQueue;
		std::vector<std::vector<float>> bestG(height, std::vector<float>(width, kInf));
		std::vector<std::vector<bool>> closed(height, std::vector<bool>(width, false));
		std::vector<std::vector<Vector2>> parent(height, std::vector<Vector2>(width, Vector2(-1, -1)));

		static const std::vector<Direction> directions = {
			{ Vector2(0, -1), 1.0f }, // 상.
			{ Vector2(-1, 0), 1.0f }, // 좌.
			{ Vector2(1, 0), 1.0f },   // 우.
			{ Vector2(0, 1), 1.0f },  // 하.
			{ Vector2(1, -1), DIAGONAL_COST}, // 우상.
			{ Vector2(-1, -1), DIAGONAL_COST}, // 좌상.
			{ Vector2(1, 1), DIAGONAL_COST}, // 우하.
			{ Vector2(-1, 1), DIAGONAL_COST} // 좌하.
		};

		bestG[start.y][start.x] = 0.0f;
		openQueue.push(OpenEntry{ GetHCost(start, target, hNoise), 0.0f, start });

		while (!openQueue.empty())
		{
			OpenEntry current = openQueue.top();
			openQueue.pop();

			const int cx = current.position.x;
			const int cy = current.position.y;

			if (closed[cy][cx])
			{
				continue;
			}

			if (current.gCost > bestG[cy][cx])
			{
				continue;
			}

			if (current.position == target)
			{
				Vector2 cursor = target;
				while (cursor != start)
				{
					outPath.push_back(cursor);
					const Vector2& next = parent[cursor.y][cursor.x];
					if (next.x < 0 || next.y < 0)
					{
						outPath.clear();
						return false;
					}
					cursor = next;
				}
				std::reverse(outPath.begin(), outPath.end());
				return true;
			}

			closed[cy][cx] = true;

			for (const Direction& dir : directions)
			{
				Vector2 nextPosition = current.position + dir.offset;
				const int nextX = nextPosition.x;
				const int nextY = nextPosition.y;

				if (IsGridRange(nextPosition, grid))
				{
					continue;
				}

				if (grid[nextY][nextX] == 1 || grid[nextY][nextX] == 2)
				{
					continue;
				}

				if (dir.offset.x != 0 && dir.offset.y != 0)
				{
					Vector2 adjacent1 = nextPosition - Vector2(dir.offset.x, 0);
					Vector2 adjacent2 = nextPosition - Vector2(0, dir.offset.y);
					if (IsGridRange(adjacent1, grid) || IsGridRange(adjacent2, grid))
					{
						continue;
					}
					if (grid[adjacent1.y][adjacent1.x] == 1 || grid[adjacent1.y][adjacent1.x] == 2 ||
						grid[adjacent2.y][adjacent2.x] == 1 || grid[adjacent2.y][adjacent2.x] == 2)
					{
						continue;
					}
				}

				float nextGCost = bestG[cy][cx] + dir.cost + cellNoise[nextY][nextX];
				if (nextGCost >= bestG[nextY][nextX])
				{
					continue;
				}

				bestG[nextY][nextX] = nextGCost;
				parent[nextY][nextX] = current.position;

				const float hCost = GetHCost(nextPosition, target, hNoise);
				openQueue.push(OpenEntry{ nextGCost + hCost, nextGCost, nextPosition });
			}
		}

		return false;
	}

	bool Navigation::IsGridRange(const Vector2& position, const std::vector<std::vector<int>>& grid) const
	{
		if (grid.empty() || grid[0].empty())
		{
			return true;
		}

		if (position.x < 0 || position.y < 0 || position.y >= grid.size() || position.x >= grid[0].size())
		{
			return true;
		}

		return false;
	}

	float Navigation::GetHCost(const Vector2& position, const Vector2& target, float noise) const
	{
		float dx = static_cast<float>(std::abs(position.x - target.x));
		float dy = static_cast<float>(std::abs(position.y - target.y));

		const float tie = 1.0f + (1.0f / 1000.0f);
		return ((dx + dy) + (DIAGONAL_COST - 2.0f) * std::min(dx, dy)) * tie + noise;
	}
}

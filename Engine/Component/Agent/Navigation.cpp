#define NOMINMAX
#include "Navigation.h"
#include "Util/Util.h"

#include <cmath>
#include <algorithm>
#define DIAGONAL_COST 1.41421356f

namespace Wanted
{
	Navigation::~Navigation()
	{
		ClearLists();
	}

	bool Navigation::FindPath(const Vector2& start, const Vector2& target, const std::vector<std::vector<int>>& grid, std::vector<Vector2>& outPath)
	{
		ClearLists();

		if (IsGridRange(start, grid) || IsGridRange(target, grid))
		{
			return false;
		}

		startNode = new Node(start);

		openList.emplace_back(startNode);

		std::vector<Direction> directions = {
			{ Vector2(0, -1), 1.0f }, // 상.
			{ Vector2(-1, 0), 1.0f }, // 좌.
			{ Vector2(1, 0), 1.0f },   // 우.
			{ Vector2(0, 1), 1.0f },  // 하.
			{ Vector2(1, -1), DIAGONAL_COST}, // 우상.
			{ Vector2(-1, -1), DIAGONAL_COST}, // 좌상.
			{ Vector2(1, 1), DIAGONAL_COST}, // 우하.
			{ Vector2(-1, 1), DIAGONAL_COST} // 좌하.
		};

		while (!openList.empty())
		{
			Node* lowCostNode = openList[0];
			for (Node* node : openList)
			{
				if (node->fCost() < lowCostNode->fCost())
				{
					lowCostNode = node;
				}
			}

			auto currentNode = lowCostNode;

			if (currentNode->position == target)
			{
				outPath = ConstructPath(currentNode);
				return true;
			}

			for (int i = 0; i < openList.size(); ++i)
			{
				if (openList[i] == currentNode)
				{
					openList.erase(openList.begin() + i);
					break;
				}
			}

			closedList.emplace_back(currentNode);

			for (const auto& dir : directions)
			{
				Vector2 nextPosition = currentNode->position + dir.offset;

				int nextX = nextPosition.x;
				int nextY = nextPosition.y;

				if (IsGridRange(nextPosition, grid))
				{
					continue;
				}

				if (grid[nextY][nextX] == 2 || grid[nextY][nextX] == 1)
				{
					continue;
				}

				float nextGCost = currentNode->gCost + dir.cost;

				if (IsCloseToTarget(nextPosition, nextGCost))
				{
					continue;
				}

				Node* nextNode = new Node(nextPosition, currentNode);
				nextNode->gCost = nextGCost;
				nextNode->hCost = GetHCost(nextPosition, target);

				if (dir.offset.x != 0 && dir.offset.y != 0)
				{
					Vector2 adjacent1 = nextNode->position - Vector2(dir.offset.x, 0);
					Vector2 adjacent2 = nextNode->position - Vector2(0, dir.offset.y);
					if (IsGridRange(adjacent1, grid) || IsGridRange(adjacent2, grid))
					{
						SafeDelete(nextNode);
						continue;
					}
					if (grid[adjacent1.y][adjacent1.x] == 1 || grid[adjacent1.y][adjacent1.x] == 2 ||
						grid[adjacent2.y][adjacent2.x] == 1 || grid[adjacent2.y][adjacent2.x] == 2)
					{
						SafeDelete(nextNode);
						continue;
					}
				}

				Node* openNode = nullptr;
				for (const auto& node : openList)
				{
					if (node->position == nextNode->position)
					{
						openNode = node;
						break;
					}
				}

				if (openNode)
				{
					if (nextNode->gCost < openNode->gCost)
					{
						openNode->parent = currentNode;
						openNode->gCost = nextGCost;
						openNode->hCost = nextNode->hCost;
					}
					SafeDelete(nextNode);
					continue;
				}
				openList.emplace_back(nextNode);
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

	float Navigation::GetHCost(const Vector2& position, const Vector2& target) const
	{
		float dx = static_cast<float>(std::abs(position.x - target.x));
		float dy = static_cast<float>(std::abs(position.y - target.y));

		return (dx + dy) + (DIAGONAL_COST - 2.0f) * std::min(dx, dy);
	}

	bool Navigation::IsCloseToTarget(const Vector2& position, float cost) const
	{
		for (Node* node : closedList)
		{
			if (node->position == position && node->gCost <= cost)
			{
				return true;
			}
		}

		return false;
	}

	std::vector<Vector2> Navigation::ConstructPath(Node* target)
	{
		std::vector<Vector2> path;
		Node* currentNode = target;
		while (currentNode->parent != nullptr)
		{
			path.push_back(currentNode->position);
			currentNode = currentNode->parent;
		}
		std::reverse(path.begin(), path.end());
		return path;
	}
	void Navigation::ClearLists()
	{
		if (openList.size() > 0)
		{
			for (Node* node : openList)
			{
				SafeDelete(node);
			}
			openList.clear();
		}

		if (closedList.size() > 0)
		{
			for (Node* node : closedList)
			{
				SafeDelete(node);
			}
			closedList.clear();
		}

		startNode = nullptr;
	}
}
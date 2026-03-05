#pragma once

#include "Math/Vector2.h"

namespace Wanted
{
	struct Node
	{
		Node(const Vector2& position, Node* parent = nullptr)
			: position(position), parent(parent)
		{

		}

		Vector2 position;

		float gCost = 0.0f;
		float hCost = 0.0f;
		float fCost() const { return gCost + hCost; }

		Node* parent = nullptr;
	};
}

#pragma once

#include <cstdint>

using LayerMask = std::uint32_t;
using CollisionID = std::uint32_t;

enum class CollisionLayer : CollisionID
{
	None = 0,
	Bullet = 1,
	Enemy = 2,
};

constexpr LayerMask GetLayerMask(CollisionLayer layer)
{
	if (layer == CollisionLayer::None)
	{
		return LayerMask{ 0 };
	}

	const auto layerValue = static_cast<LayerMask>(layer);
	return LayerMask{ 1 } << layerValue;
}

enum class EventType
{
	Enter,
	Stay,
	Exit,
};

struct CollisionEvent
{
	EventType type;
	CollisionID selfID;
	CollisionID otherID;
};

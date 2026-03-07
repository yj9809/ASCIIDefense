#include "Tower.h"

Tower::Tower(const Vector2& position)
	: Actor("###\n#T#\n###", Vector2(position.x - 1, position.y - 1), Color::Yellow)
{
	sortingOrder = 1;
}

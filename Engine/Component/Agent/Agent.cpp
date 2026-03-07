#include "Agent.h"
#include "Actor/Actor.h"
#include "Util/Util.h"

namespace Wanted
{
	Agent::Agent(Actor* owner, std::vector<Vector2> path)
		: Component(owner), path(std::move(path))
	{
		
	}

	void Agent::Move()
	{
		if (currentPathIndex >= path.size())
		{
			return;
		}

		if(isMoving)
			owner->SetPosition(path[currentPathIndex++]);		

		isMoving = false;
	}

	Vector2 Agent::NextPosition() const
	{
		if (currentPathIndex < path.size())
		{
			return path[currentPathIndex];
		}

		return Vector2();
	}

	bool Agent::HasNext() const
	{
		if (currentPathIndex < path.size())
		{
			return true;
		}

		return false;
	}
}

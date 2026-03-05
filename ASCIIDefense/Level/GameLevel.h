#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"

#include <vector>

namespace Wanted
{
	class GameLevel : public Level
	{
		RTTI_DECLARATIONS(GameLevel, Level)

	public:
		GameLevel();

		void LoadMap(const char* mapFile);

		std::vector<std::vector<int>> GetGrid() const { return grid; }
	private:
		void SetGrid(std::vector<int> line);

	private:
		std::vector<std::vector<int>> grid;
	};
}


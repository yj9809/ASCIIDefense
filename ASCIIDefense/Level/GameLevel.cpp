#include "GameLevel.h"
#include "Actor/Bullet.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Ground.h"

#include <iostream>

namespace Wanted
{
	GameLevel::GameLevel()
	{
		LoadMap("map.txt");
	}


	void GameLevel::LoadMap(const char* mapFile)
	{
		char path[2048] = {};
		sprintf_s(path, 2048, "../Assets/Maps/%s", mapFile);

		FILE* file = nullptr;
		fopen_s(&file, path, "rt");

		if (!file)
		{
			std::cerr << "GameLevel::LoadMap - Failed to open map file.\n";

			__debugbreak();
		}

		fseek(file, 0, SEEK_END);

		size_t fileSize = ftell(file);

		rewind(file);

		char* buffer = new char[fileSize + 1];

		size_t readSize = fread(buffer, sizeof(char), fileSize, file);

		int maxWidth = 0;
		int index = 0;

		Vector2 position;

		/*std::vector<Vector2> startPositions;
		std::vector<Vector2> endPositions;*/

		std::vector<int> line;

		while (true)
		{
			if (index >= fileSize)
			{
				break;
			}

			char mapCharacter = buffer[index];
			++index;

			if (mapCharacter == '\n')
			{
				maxWidth = max(maxWidth, position.x);
				++position.y;
				position.x = 0;
				SetGrid(line);
				line.clear();
				continue;
			}
			switch (mapCharacter)
			{
			case '#':
				AddNewActor(new Wall(position));
				line.push_back(1);
				break;
			case '.':
				AddNewActor(new Ground(position));
				line.push_back(0);
				break;
			}
			++position.x;
		}

		fclose(file);
	}

	void GameLevel::SetGrid(std::vector<int> line)
	{
		grid.push_back(line);
	}
}
#include "Engine/Engine.h"
#include "Level/GameLevel.h"

using namespace Wanted;

int main()
{
	Engine engine;

	// 새 레벨 설정.
	GameLevel* gameLevel = new GameLevel();
	engine.SetNewLevel(gameLevel);

	engine.Run();

}
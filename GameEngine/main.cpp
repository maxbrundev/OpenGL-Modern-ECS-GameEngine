#include "game_engine_stdafx.h"

#include "GameEngine.h"

using namespace ECS;
using namespace Utils;

int main(int argc, char* argv[])
{
	{
		GameEngine gameEngine;
		gameEngine.Init();
		gameEngine.Setup();
		gameEngine.Run();
		gameEngine.Close();
	}
	return 0;
}
#include "Game.h"
#include <Level/GameLevel.h>

Game::Game()
{
	AddNewLevel<GameLevel>();
}

#include "Game.h"
#include <Level/GameLevel.h>
#include <Level/GameOverLevel.h>
#include <Level/GameClearLevel.h>
#include <Level/MainLevel.h>


Game::Game()
{
	levelList.emplace_back(std::make_shared<MainLevel>());
	levelList.emplace_back(std::make_shared<GameLevel>());
	levelList.emplace_back(std::make_shared<GameOverLevel>());
	levelList.emplace_back(std::make_shared<GameClearLevel>());
	state = State::GameLevel;

	mainLevel = levelList[static_cast<int>(state)];
	//AddNewLevel<GameLevel>();
}

void Game::SetGameOverLevel()
{ // 겜오버로 ㄱㄱ
	state = State::GameOverLevel;
	mainLevel = levelList[static_cast<int>(state)];
}

#include "GameOverLevel.h"
#include <Render/Renderer.h>
#include <Input/Input.h>
#include <Game/Game.h>
using namespace Craft;
void GameOverLevel::OnInitialized()
{
	
}

void GameOverLevel::Draw()
{
	Renderer::Get().Submit(L"GameOver...", Vector2(5, 5));
	Renderer::Get().Submit(L"Regame: Enter/Space", Vector2(10, 10));
}

void GameOverLevel::Tick(float deltaTime)
{
	Game& game = dynamic_cast<Game&>(Engine::Get());
	if (Input::Get().GetKeyDown(VK_RETURN) || Input::Get().GetKeyDown(VK_SPACE))
		game.SetGameLevel();
}


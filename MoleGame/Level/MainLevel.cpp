#include "MainLevel.h"
#include <Render/Renderer.h>
#include <Input/Input.h>
#include <Game/Game.h>

using namespace Craft;
void MainLevel::Draw()
{
	Renderer::Get().Submit(L"Mole Game", Vector2(5, 5));
	Renderer::Get().Submit(L"Press Enter...", Vector2(5, 8));

}

void MainLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
	Game& game = dynamic_cast<Game&>(Engine::Get());
	if (Input::Get().GetKeyDown(VK_RETURN) || Input::Get().GetKeyDown(VK_SPACE))
		game.SetGameLevel();
}


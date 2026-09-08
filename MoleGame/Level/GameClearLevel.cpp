#include "GameClearLevel.h"
#include <Render/Renderer.h>
using namespace Craft;
void GameClearLevel::Draw()
{
	Renderer::Get().Submit(L"Game Clear!", Vector2(5, 5));

}

#include "Mole.h"
#include <Input/Input.h>

using namespace Craft;
Mole::Mole(const Vector2& position) : Actor(L"P", position, Color::CYAN)
{
}

void Mole::Tick(float deltaTime)
{
	
	if (Input::Get().GetKey(VK_UP))
		position = position + arrow.up;
	if (Input::Get().GetKey(VK_DOWN))
		position = position + arrow.down;
	if (Input::Get().GetKey(VK_LEFT))
		position = position + arrow.left;
	if (Input::Get().GetKey(VK_RIGHT))
		position = position + arrow.right;

}

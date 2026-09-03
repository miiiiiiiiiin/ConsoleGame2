#include "Mole.h"
#include <Input/Input.h>
#include <Level/GameLevel.h>

using namespace Craft;
Mole::Mole(const Vector2& position) : Actor(L"P", position, Color::CYAN), timer(0.2f)
{
}

void Mole::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	timer.Tick(deltaTime);
	if (!timer.IsTargetTime()) return;

	// 플레이어 이동
	GetKey('W');
	GetKey('A');
	GetKey('S');
	GetKey('D');

	timer.Reset();

}

void Mole::GetKey(int keyCode)
{
	Vector2 newPosition;
	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	if (Input::Get().GetKey(keyCode))
	{
		switch (keyCode)
		{
		case 'W':
			Input::Get().GetKey(keyCode);
			newPosition = position + arrow.up;
			if (level->CanMove(position, newPosition))
				position = newPosition;
			break;
		case 'A':
			newPosition = position + arrow.left;
			if (level->CanMove(position, newPosition))
				position = newPosition;
			break;
		case 'S':
			newPosition = position + arrow.down;
			if (level->CanMove(position, newPosition))
				position = newPosition;
			break;
		case 'D':
			newPosition = position + arrow.right;
			if (level->CanMove(position, newPosition))
				position = newPosition;
			break;
		}
	}
}

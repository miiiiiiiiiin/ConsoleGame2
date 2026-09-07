#include "Mole.h"
#include <Input/Input.h>
#include <Level/GameLevel.h>

using namespace Craft;
Mole::Mole(const Vector2& position) : Actor(L"P", position, Color::CYAN), timer(0.05f)
{
	sortingOrder = 10;
}

void Mole::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());

	timer.Tick(deltaTime);
	if (!timer.IsTargetTime()) return;

	//// 플레이어 이동
	GetKey('W');
	GetKey('A');
	GetKey('S');
	GetKey('D');

	level->SetTargetPosition(position);
	// 플레이어가 오른쪽 벽 넘으면 막고, 왼쪽으로 밀리면 겜 오버.
	if (position.x >= level->GetCameraPosition().x + 30)
		position.x = level->GetCameraPosition().x + 30;
	else if(position.x <= level->GetCameraPosition().x)
		level->SetGameOver(true);


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

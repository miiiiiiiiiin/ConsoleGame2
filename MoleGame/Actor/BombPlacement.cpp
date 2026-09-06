#include "BombPlacement.h"
#include <Input/Input.h>
#include <Level/GameLevel.h>
#include <Actor/Bomb.h>
#include <iostream>

using namespace Craft;

BombPlacement::BombPlacement(const Craft::Vector2 position)
	: Actor(L"⮟", position, Color::BLUE), timer(0.2f)
{
	sortingOrder = 7;
}

void BombPlacement::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	// 마우스 클릭 이벤트 발생하면 폭탄 설치하는 로직..
	// 마우스 올린 위치와 액터 블록의 위치를 비교하여 
	// 흙 블록일 경우: 설치가능, Blue / 벽일 경우: 설치불가, Red 
	// gamelevel에서 bool 함수를 들고와서 비교한다..

	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	// 카메라 좌표 + 월드 좌표 = 현재 좌표
	position = level->GetCameraPosition() + Input::Get().GetMousePosition();
			//OutputDebugStringA("Timer ticked\n");
	if (level->IsBombBlock())
	{
		color = Color::BLUE;
		// 폭탄 설치
		if (Input::Get().GetKeyDown(VK_LBUTTON))
		{
			// Bomb클래스 spawnActor호출
			level->SpawnActor<Bomb>(position);
			// Tick 시작.
			
			// 마우스 표시를 x로 설정하고 터치못하도록 ㄱㄱ
			//Destroy();
			// 시간 됐으면 다시 시작 

		}
	}
	else
	{
		color = Color::RED;
	}

}

#include "Bomb.h"
using namespace Craft;

Bomb::Bomb(const Craft::Vector2& position)
	: Actor(L"◉", position, Color::PURPLE), timer(0.6f)
{

}

void Bomb::Tick(float deltaTime)
{

	timer.Tick(deltaTime);
	if (!timer.IsTargetTime()) return;

	// 쿼드트리 확인해서 터트릴거 터트리기.????
	if (aniCount <= 2)
	{
		image = animation[aniCount++];
	}

	timer.Reset();
}

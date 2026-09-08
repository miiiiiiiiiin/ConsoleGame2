#include "Goal.h"
#include <Actor/Actor.h>

Goal::Goal(const Vector2 position)
	: Actor(L"|", position, Color::SKY)
{
}

// Todo: Mole클래스에 겜오버 넣었다면 틱 삭제
void Goal::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	//골에 닿으면 겜 클리어 화면으로 이동
}

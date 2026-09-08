#pragma once
#include <Actor/Actor.h>
#include <Util/Timer.h>
#include <Actor/Mole.h>
using namespace Craft;
//struct Arrow
//{
//	Vector2 up = Vector2(0, -1);
//	Vector2 down = Vector2(0, 1);
//	Vector2 left = Vector2(-1, 0);
//	Vector2 right = Vector2(1, 0);
//};

// 마우스로 폭탄 위치 설정하는 클래스.
class BombPlacement : public Craft::Actor
{
	TYPE_DECLARATIONS(BombPlacement, Actor);
public:
	BombPlacement(const Craft::Vector2 position);

private:
	virtual void Tick(float deltaTime) override;
	// 키 눌림 이동

private:
	Timer timer;

	Arrow arrow;
};


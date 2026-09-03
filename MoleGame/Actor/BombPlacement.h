#pragma once
#include <Actor/Actor.h>
#include <Util/Timer.h>

// 마우스로 폭탄 위치 설정하는 클래스.
class BombPlacement : public Craft::Actor
{
	TYPE_DECLARATIONS(BombPlacement, Actor);
public:
	BombPlacement(const Craft::Vector2 position);

	virtual void Tick(float deltaTime) override;

private:
	Timer timer;

};


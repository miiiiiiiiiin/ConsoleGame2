#pragma once
#include <Actor/Actor.h>
#include <Util/Timer.h>

using namespace Craft;

struct Arrow 
{
	Vector2 up = Vector2(0, -1);
	Vector2 down = Vector2(0, 1);
	Vector2 left = Vector2(-1, 0);
	Vector2 right = Vector2(1, 0);
};

class Mole : public Craft::Actor
{
	TYPE_DECLARATIONS(Mole, Actor);
public:
	Mole(const Vector2& position);

private:
	// 이동 로직..
	virtual void Tick(float deltaTime) override;

	// 키 눌림 이동
	void GetKey(int keyCode);

private:
	Timer timer;
	Arrow arrow;
};


#pragma once
#include <Actor/Actor.h>
#include <Util/Timer.h>

class Enemy : public Craft::Actor
{
public:
	TYPE_DECLARATIONS(Enemy, Actor);

	Enemy(const Craft::Vector2 position);

	virtual void Tick(float deltaTime) override;


private:
	Timer timer;
	int count = 0;

};


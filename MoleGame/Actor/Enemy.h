#pragma once
#include <Actor/Actor.h>

class Enemy : public Craft::Actor
{
public:
	TYPE_DECLARATIONS(Enemy, Actor);

	Enemy(const Craft::Vector2 position);

	virtual void Tick(float deltaTime) override;


private:


};


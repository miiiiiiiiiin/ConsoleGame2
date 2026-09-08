#pragma once
#include <Actor/Actor.h>
using namespace Craft;
class Goal : public Actor
{
	TYPE_DECLARATIONS(Goal, Actor);
public:
	Goal(const Vector2 position);
private:
	virtual void Tick(float deltaTime) override;

private:
	// 멤버변수


};


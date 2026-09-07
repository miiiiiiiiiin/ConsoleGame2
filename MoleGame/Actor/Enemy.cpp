#include "Enemy.h"
#include <Level/GameLevel.h>
#include <Actor/Mole.h>
using namespace Craft;
Enemy::Enemy(const Craft::Vector2 position)
	: Actor(L"E", position, Color::RED), timer(0.2f)
{
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	timer.Tick(deltaTime);
	if (!timer.IsTargetTime()) return;
	count = 1;
	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	level->SetStartPosition(position);
	std::vector<Craft::Vector2> result = level->SetAstar();
	if (result.size() < count) return;
	position = result[count++];
	// 충돌 처리.
	if (level->FindActorAt<Mole>(position))
	{
		level->SetGameOver(true);
	}
	timer.Reset();

}

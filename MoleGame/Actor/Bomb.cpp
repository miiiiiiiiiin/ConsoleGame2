#include "Bomb.h"
#include <Level/GameLevel.h>

using namespace Craft;

Bomb::Bomb(const Craft::Vector2& position)
	: Actor(L"◉", position, Color::PURPLE), timer(0.2f)
{

}

void Bomb::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	timer.Tick(deltaTime);
	if (!timer.IsTargetTime()) return;

	 //쿼드트리 확인해서 터트릴거 터트리기.????

	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	std::vector<std::shared_ptr<Actor>> result;
	level->BombBlockByQuadTree(position, result);

	if (!isAniEnd)
	{
		for (const std::shared_ptr<Actor>& actor : result)
		{
			actor->ChangeImage(animation[aniCount]);
		}
		image = animation[aniCount];
	}
	else
	{
		for (const std::shared_ptr<Actor>& actor : result)
		{
			actor->Destroy();
		}
		Destroy();
	}
	aniCount++;
	isAniEnd = aniCount > 2 ? true : false;
	timer.Reset();
}

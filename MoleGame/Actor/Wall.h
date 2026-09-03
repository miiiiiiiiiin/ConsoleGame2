#pragma once
#include <Actor/Actor.h>
#include <Core/ActorFactory.h>

class Wall: public Craft::Actor
{
	//struct WallRegister
	//{
	//	WallRegister()
	//	{
	//		ActorFactory::Register('#', [](Craft::Level& level, const Craft::Vector2& position)
	//			{
	//				level.SpawnActor<Wall>(position);
	//			});
	//	}
	//} wallRegister; // 뒤에 객체이름 적으면 바로 전역 객체 생성됨. (namespace로 이 파일 안에서만 돌도록..)

	TYPE_DECLARATIONS(Wall, Actor);
public:
	Wall(const Craft::Vector2& position);

private:
	
};



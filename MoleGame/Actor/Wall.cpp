#include "Wall.h"

//~Wall() {}
using namespace Craft;
Wall::Wall(const Vector2& position)
	: Actor(L"█", position, Color::WHITE)
{
	sortingOrder = 5;
}

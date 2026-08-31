#pragma once
#include <unordered_map>
#include <functional>
#include <Actor/Actor.h>
//#include <Level/Level.h>
using namespace Craft;
// 액터들을 
class ActorFactory
{
public:
	
	using CreateFunc = std::function<void(Level&, const Vector2&)>;
private:
	//static std::unordered_map<char, CreateFunc> 
};
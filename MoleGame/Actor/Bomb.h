#pragma once
#include <Actor/Actor.h>
#include <Util/Timer.h>

class Bomb : public Craft::Actor
{
	//struct Bombani
	//{
	//	const wchar_t* a = L"✸";
	//	const wchar_t* b = L"✦";
	//	const wchar_t* a = L"·";
	//};
	TYPE_DECLARATIONS(Bomb, Actor);
public:
	Bomb(const Craft::Vector2& position);

	virtual void Tick(float deltaTime) override;
private:
	Timer timer;
	const wchar_t* animation[3]{
		L"✸",
		L"✦",
		L"·"
	};
	int aniCount = 0;

};


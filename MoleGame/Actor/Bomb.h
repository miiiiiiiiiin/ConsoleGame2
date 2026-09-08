#pragma once
#include <Actor/Actor.h>
#include <Util/Timer.h>
#include <vector>

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
	//virtual void BeginPlay() override;
	Bomb(const Craft::Vector2& position);

private:
	virtual void Tick(float deltaTime) override;

private:
	Timer timer;
	const wchar_t* animation[3]{
		L"✸",
		L"✦",
		L"·"
	};
	int aniCount = 0;
	bool isAniEnd = false;
	std::vector<std::shared_ptr<Craft::Actor>> result;

};


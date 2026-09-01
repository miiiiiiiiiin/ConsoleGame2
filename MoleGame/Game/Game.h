#pragma once
#include <Engine/Engine.h>

class Game : public Craft::Engine
{
public:
	Game();
	~Game() = default;

	// 화면 가로세로 크기 가져오는 게터
	int GetFrameWidth() { return GetWidth(); }
	int GetFrameHeight() { return GetHeight(); }
	int GetFrameRate() { return GetRate(); }
private:

};


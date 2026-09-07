#pragma once
#include <Engine/Engine.h>
#include <vector>

class Game : public Craft::Engine
{
	enum class State
	{
		mainLevel = 0,
		GameLevel = 1,
		GameOverLevel,
		GameClearLevel
	};

public:
	Game();
	~Game() = default;

	// 화면 가로세로 크기 가져오는 게터
	int GetFrameWidth() { return GetWidth(); }
	int GetFrameHeight() { return GetHeight(); }
	int GetFrameRate() { return GetRate(); }

	// 게임-> 게임오버
	void SetGameOverLevel();
private:
	// 레벨 리스트
	std::vector<std::shared_ptr<Level>> levelList;

	State state;


};


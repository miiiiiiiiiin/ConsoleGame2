#pragma once
#include <Level/Level.h>

class GameLevel : public Craft::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	

private:
	/* 멤버함수 */

	// 레벨 초기화 함수
	virtual void OnInitialized() override;
	// update
	virtual void Tick(float deltaTime) override;
	// 맵 올리기
	virtual void Draw() override;
	// 맵불러오기
	void LoadMap(const std::string& filename);

private:
	/* 멤버변수 */


};


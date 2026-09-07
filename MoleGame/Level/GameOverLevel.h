#pragma once
#include <Level/Level.h>

class GameOverLevel : public Craft::Level
{
public:
	TYPE_DECLARATIONS(GameOverLevel, Level);

private:
	virtual void Tick(float deltaTime) override;
	virtual void OnInitialized() override;
	virtual void Draw() override;


};


#pragma once
#include <Level/Level.h>

class MainLevel : public Craft::Level
{
public:
	virtual void Draw() override;
	virtual void Tick(float deltaTime) override;
private:

};


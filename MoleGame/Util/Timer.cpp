#include "Timer.h"

Timer::Timer(float time)
	: elapsedTime(0.0f) , targetTime(time)
{
}

void Timer::Tick(float deltaTime)
{
	elapsedTime += deltaTime;
	if (elapsedTime >= targetTime) elapsedTime = targetTime;
}

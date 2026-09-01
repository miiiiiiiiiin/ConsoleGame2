#pragma once
class Timer
{
public:
	Timer(float time);
	// 타겟 시간 세팅
	void TimerSetting(float time) { targetTime = time; }

	// 리셋
	void Reset() { targetTime = 0; elapsedTime = 0; }

	// 타이머 시작
	void Tick(float deltaTime);


	// 타이머 끝
	bool IsTargetTime() const { return elapsedTime >= targetTime; }
private:
	float targetTime = 0.0f;
	float elapsedTime = 0.0f;

};


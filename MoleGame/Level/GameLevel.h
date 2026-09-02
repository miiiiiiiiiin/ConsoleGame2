#pragma once
#include <Level/Level.h>
#include <Math/Vector2.h>

class GameLevel : public Craft::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	//Vector2 GetCameraPosition() const { return cameraPosition; }

	// 카메라 좌표 변경 함수
	void UpdateCamera(float deltaTime);

	// 이동 가능 
	bool CanMove(const Craft::Vector2& playerPosition, const Craft::Vector2& nextPosition);
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

	// framerate 확인
	void FrameRate(float deltaTime);


private:
	/* 멤버변수 */
	// 카메라 포지션
	//Vector2 cameraPosition;

	// 카메라 이동시 
	float cameraElapsedTime = 0.0f;
	// 카메라를 초당 몇칸씩 움직일지..
	float cameraSpeed = 10.0f;

	float FPS= 0.0f;

	// 레알 정수형 카메라 위치
	int CameraX = 0.0f;
	// 카메라용 소수점 보관소
	float cameraAccumX = 0.0f;
};


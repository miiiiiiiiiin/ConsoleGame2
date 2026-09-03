#pragma once
#include <Level/Level.h>
#include <Math/Vector2.h>
#include <Input/Input.h>

#include <unordered_map>

class GameLevel : public Craft::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	//Vector2 GetCameraPosition() const { return cameraPosition; }

	// 카메라 좌표 변경 함수
	void UpdateCamera(float deltaTime);

	// 이동 가능 
	bool CanMove(const Craft::Vector2& playerPosition, const Craft::Vector2& nextPosition);

	// 부서도 되는 블럭인지 판정
	bool IsBombBlock();

	// 쿼드트리 판정 함수(폭탄 설치 시 실행)
	void BombBlockByQuadTree();

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

	// 출력 사이즈에 맞는 액터만 가져오도록 하는 함수
	// 벡터인 액터 리스트에 화면 크기의 액터만 담는다. -> 화면이 1 이동할 때마다 세로 한 줄을 빼고 세로 한 줄을 담는다 



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

	std::unordered_map<int64_t, std::shared_ptr<Craft::Actor>> blockGrid;

	int64_t EncodePos(int x, int y) const
	{
		return (int64_t)x * 100000 + y;
	}

};


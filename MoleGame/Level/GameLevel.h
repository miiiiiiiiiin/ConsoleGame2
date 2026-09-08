#pragma once
#include <Level/Level.h>
#include <Math/Vector2.h>
#include <Input/Input.h>
#include <Actor/QuadTree/QuadTreeNode.h>
#include <Actor/AStar/AStar.h>
#include <Util/Timer.h>
#include <unordered_map>

class GameLevel : public Craft::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	// 카메라 좌표 변경 함수
	void UpdateCamera(float deltaTime);

	// 이동 가능 
	bool CanMove(const Craft::Vector2& playerPosition, const Craft::Vector2& nextPosition);

	// 부서도 되는 블럭인지 판정
	bool IsBombBlock();

	// 쿼드트리 판정 함수(폭탄 설치 시 실행)
	void BombBlockByQuadTree(const Craft::Vector2 Bombposition, std::vector<std::shared_ptr<Actor>>& result);

	// 에이스타 그리드 만들어서 경로 제작.
	// 디버그모드 그리기는 따로.
	std::vector<Craft::Vector2>& SetAstar();

	void SetStartPosition(Craft::Vector2 position) { startPosition = position; }
	void SetTargetPosition(Craft::Vector2 position) { targetPosition = position; }

	// 겜오버시 트리거 바꾸기
	void SetGameOver(bool gameOver) { GameOver = gameOver; }
	void SetGameClear(bool gameclear) { GameClear = gameclear; }

	// 몬스터 이동 시작
	void SetEnemydepart(bool depart) { isEnemydepart = depart; }
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
	// 벡터인 액터 리스트에 화면 크기의 액터만 담는다
	void UpdateVisibleActors();

	void DebugMode();

	//디버그모드 토글
	void isDeBugModeToggle();

	// 미니맵 위치 변경
	void minimapVecChange();

private:
	/* 멤버변수 */
	
	float cameraElapsedTime = 0.0f;

	// 카메라를 초당 몇칸씩 움직일지..
	float cameraSpeed = 3.5f; 

	float FPS= 0.0f;

	// 레알 정수형 카메라 위치
	int CameraX = 0;
	// 카메라용 소수점 보관소
	float cameraAccumX = 0.0f;

	// 화면에 출력시킬 액터 고를때 쓰는 해시테이블(공간해싱)
	std::unordered_map<int64_t, std::shared_ptr<Craft::Actor>> blockGrid;
	// 좌표를 해싱시키는 함수..
	int64_t EncodePos(int x, int y) const
	{
		return (int64_t)x * 31 + y;
	}

	//쿼드트리 루트 노드(호출될때마다 초기화됨..)
	std::shared_ptr<QuadTreeNode> root;

	// 디버그모드
	bool isDebugMode = false;

	Craft::Vector2 BombPositionForDebug;

	// 에이스타 찾을때 전달할 그리드
	// 0: 장애물벽. / 1: 움직일 수 있는 곳 // 2: 방문 노드 // 3: 시작.도착지
	std::vector<std::pair<Vector2, int>> grid;

	// 에이스타용 목표위치 시작위치
	Craft::Vector2 startPosition;
	Craft::Vector2 targetPosition;

	std::shared_ptr<AStar> astar;
	std::vector<Craft::Vector2> path;

	// 겜오버 트리거
	bool GameOver = false;
	// 겜클리어 트리거
	bool GameClear = false;

	// 겜 시작 전 트리거
	bool GameStart = false;
	int StartCount = 3;

	Timer timer;

	// 
	bool isEnemydepart = false;
	Timer EnemyTimer;

	// 미니맵 위치용
	Vector2 nowDir = Vector2(2, 11);

	// 미니맵 두더지 위치 비교용
	Vector2 changeTargetPos = Vector2(0,0);
};


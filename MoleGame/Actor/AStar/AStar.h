#pragma once
#include "Node.h"
#include <vector>

class AStar
{
private:
	struct Direction
	{ // 이동 방향과 해당 방향의 이동 비용
		int x;
		int y;
		float cost;
	};

public:
	/* ------ 공개 메소드 ----------- */

	AStar();
	~AStar();
	// 시작위치 - 목표위치 경로 탐색
	bool FindPath(const Craft::Vector2& startPosition,
		const Craft::Vector2& goalPosition,
		std::vector<std::pair<Craft::Vector2, int>>& grid,
		std::vector<Craft::Vector2>& findPosition);

	// 탐색한 최종 경로를 그리드에 출력한다
	void DisplayGridWithPath(std::vector<std::pair<Craft::Vector2, int>>& grid, 
		const std::vector<Craft::Vector2> path, Craft::Vector2 cameraPosition);

private:
	/* ------ 비공개 메소드  ------- */

	void Clear();
	// 탐색 노드 생성
	Node* CreateNode(const Craft::Vector2& position,
		Node* parentNode = nullptr);

	// 역추적하여 경로구성
	bool ConstructPath(Node* detinationNode, std::vector<Craft::Vector2>& findPosition);

	// 목표위치까지 예상비용. 휴리스틱 계산
	float CalculateHeuristic(const Craft::Vector2& currentPosition,
		const Craft::Vector2& goalPosition) const;

	// 그리드와 좌표가 유효한지 확인
	//bool IsValidGrid(const std::vector)
	bool IsInRange(int x, int y, std::vector<std::pair<Craft::Vector2, int>>& grid);

	// 대각선 이동시 장애물 모서리 통과하는지 확인(대각선이동안함)
	//bool IsDiagonalBlocked(const )

	// open목록 closed목록 검사
	Node* FindOpenNode(int x, int y) const;
	bool IsInClosedList(int x, int y) const;
	bool IsDestination(const Node* node) const;

	// 이전 탐색 표시 지우고 그리드 출력
	void Clearvisualization(std::vector<std::pair<Craft::Vector2, int>>& grid) const;
	void DisplayGrid(const std::vector<std::pair<Craft::Vector2, int>>& grid) const;



private:
	// 동적할당한 모든 노드 소유
	std::vector<Node*> allocatedNodes;

	// 오픈리스트 / 클로즈리스트를 좌표목록으로 받는다
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	Node* startNode = nullptr;
	Node* goalNode = nullptr;

};


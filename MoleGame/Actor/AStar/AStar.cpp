#include "AStar.h"

using namespace Craft;
AStar::AStar()
{
}

AStar::~AStar()
{
	Clear();
}

bool AStar::FindPath(const Craft::Vector2& startPosition, const Craft::Vector2& goalPosition, std::vector<std::vector<int>>& grid, std::vector<Craft::Vector2>& findPosition)
{
	Clear();
	// 잘못된 좌표는 탐색하지 않는다.
	if (!IsInRange(startPosition.x, startPosition.y)
		|| !IsInRange(goalPosition.x, goalPosition.y))
		return false;

	// 현재 좌표가 이동가능하지 않다면(벽 / 블럭) 리턴
	// 게임레벨에서 모두 검사한 노드만 가져가기??
	// 겜레벨에서 에이스타 호출시 해시맵도 전달하기


	// 이전 탐색 과정의 시각화 값 제거
	Clearvisualization();

	startNode = CreateNode(startPosition);
	goalNode = CreateNode(goalPosition);


	return false;
}

void AStar::DisplayGridWithPath(const std::vector<Craft::Vector2> findPosition)
{
}

void AStar::Clear()
{
	for (Node* node : allocatedNodes)
		delete node;

	allocatedNodes.clear();
	openList.clear();
	closedList.clear();

	startNode = nullptr;
	goalNode = nullptr;
}

Node* AStar::CreateNode(const Craft::Vector2& position, Node* parentNode)
{
	Node* node = new Node(position, parentNode);
	allocatedNodes.emplace_back(node);
	
	return node;
}

bool AStar::ConstructPath(Node* detinationNode, std::vector<Craft::Vector2>& findPosition)
{
	return false;
}

float AStar::CalculateHeuristic(const Craft::Vector2& currentPosition, const Craft::Vector2& goalPosition) const
{
	return 0.0f;
}

bool AStar::IsInRange(int x, int y)
{
	return false;
}

Node* AStar::FindOpenNode(int x, int y) const
{
	return nullptr;
}

bool AStar::IsInClosedList(int x, int y) const
{
	return false;
}

bool AStar::InDestination(const Node* node) const
{
	return false;
}

void AStar::Clearvisualization() const
{
}

void AStar::DisplayGrid() const
{
}

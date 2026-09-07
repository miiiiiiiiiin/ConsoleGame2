#include "AStar.h"
#include <algorithm>
#include <iostream>
#define NOMINMAX
//#include <Windows.h>

using namespace Craft;
AStar::AStar()
{
}

AStar::~AStar()
{
	Clear();
}

bool AStar::FindPath(const Craft::Vector2& startPosition, const Craft::Vector2& goalPosition, std::vector<std::pair<Craft::Vector2, int>>& grid, std::vector<Craft::Vector2>& findPosition)
{
	Clear();

	//if(!IsValidGrid(grid))
	// 잘못된 좌표는 탐색하지 않는다.
	if (!IsInRange(startPosition.x, startPosition.y, grid)
		|| !IsInRange(goalPosition.x, goalPosition.y, grid))
		return false;

	// 시작/목표위치가 이동가능하지 않다면(벽 / 블럭) 리턴
	// 게임레벨에서 모두 검사한 노드만 가져가기??
	// 겜레벨에서 에이스타 호출시 해시맵도 전달하기
	auto it1 = std::find(grid.begin(), grid.end(), std::make_pair(startPosition, 0));
	if (it1 != grid.end())
		return false;
	auto it2 = std::find(grid.begin(), grid.end(), std::make_pair(goalPosition, 0));
	if (it2 != grid.end())
		return false;

	// 이전 탐색 과정의 시각화 값 제거
	Clearvisualization(grid);

	startNode = CreateNode(startPosition);
	goalNode = CreateNode(goalPosition);

	startNode->gCost = 0.0f;
	startNode->hCost = CalculateHeuristic(startPosition, goalPosition);
	startNode->fCost = startNode->gCost + startNode->hCost;

	openList.emplace_back(startNode);

	// 사전 비용 설정
	const std::vector<Direction> directions =
	{
		{0, -1, 1.0f}, { 0, 1, 1.0f }, // 상 하 
		{-1, 0, 1.0f}, { 1, 0, 1.0f }, // 좌 우 
	};

	// openList가 빌 때까지 비용 작은 노드 구하기
	while (!openList.empty())
	{
		Node* currentNode = openList[0];
		for (Node* node : openList)
		{// 비용이 젤 싼 노드를 currentNode에 담기
			if (node->fCost < currentNode->fCost
				|| (node->fCost == currentNode->fCost
					&& node->hCost < currentNode->hCost))
				currentNode = node;
		}
		if (IsDestination(currentNode))
		{
			// 반환은 bool 이고 최종경로는 findPosition에 담긴다.(겜레벨에서 변수만들어야함)
			return ConstructPath(currentNode, findPosition);
		}
		// 다 찾았으면 오픈목록에서 지우고 클로즈목록으로 ㄱㄱ
		auto it3 = std::find(openList.begin(), openList.end(), currentNode);
		if (it3 != openList.end())
			openList.erase(it3);

		closedList.emplace_back(currentNode);

		// 현재 위치를 기준으로 주위 방향(4) 검색
		for (const Direction& direction : directions)
		{
			// 인접한 노드 좌표 계산
			int newX = currentNode->position.x + direction.x;
			int newY = currentNode->position.y + direction.y;

			if (!IsInRange(newX, newY, grid))
				continue;
			// 이미 방문한 곳이라면건너뛰기(클로즈드리스트)
			if (IsInClosedList(newX, newY))
				continue;

			float newgCost = currentNode->gCost + direction.cost;
			// 새로 찾은 위치가 이미 오픈리스트에있다면 비용 확인 후 변경.
			Node* openNode = FindOpenNode(newX, newY);
			if (openNode)
			{
				if (newgCost < openNode->gCost)
				{
					openNode->gCost = newgCost;
					openNode->fCost = openNode->gCost + openNode->hCost;
					openNode->parentNode = currentNode;
				}
				continue;
			}

			Node* neighborNode = CreateNode(Vector2(newX, newY), currentNode);

			neighborNode->gCost = newgCost;
			neighborNode->hCost = CalculateHeuristic(neighborNode->position, goalNode->position);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			openList.emplace_back(neighborNode);
			auto it4 = std::find(grid.begin(), grid.end(), std::make_pair(Vector2(newX, newY), 1));
			if (it4 != grid.end())
			{
				it4->second = 2; // 2: 방문o
			}

			//DisplayGrid(grid);
		}
	}
	return false;
}

void AStar::DisplayGridWithPath(std::vector<std::pair<Craft::Vector2, int>>& grid,
	const std::vector<Craft::Vector2> path, Craft::Vector2 cameraPosition, const Craft::Vector2 startPosition, 
	const Craft::Vector2 goalPosition)
{
	// 기존에 시각화를 위해 사용했던 값 복구
	Clearvisualization(grid);

	// 맵 그리기(?)
	DisplayGrid(grid);

	for (const Vector2& position : path)
	{
		// 현재 포지션이 땅일때만 움직이기
		if(position != startPosition || position != goalPosition)
			Renderer::Get().Submit(L"*", position - cameraPosition, Color::GREEN);
		auto it = std::find(grid.begin(), grid.end(), std::make_pair(position, 1));
		if (it == grid.end()) continue;

	}

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
	if (!detinationNode) return false;

	// 목표 노드로부터 부모 노드를 따라 경로 역추적
	Node* current = detinationNode;

	while (current)
	{
		// 현재 노드는 경로배열에 추가
		findPosition.emplace_back(current->position);

		// 부모 노드로 이동해서 역추적
		current = current->parentNode;
	}
	// 루프가 종료되면 path에는 반대방향의 경로 정보 저장되므로 뒤집기
	std::reverse(findPosition.begin(), findPosition.end());

	return true;
}

float AStar::CalculateHeuristic(const Craft::Vector2& currentPosition, const Craft::Vector2& goalPosition) const
{
	// 현재위치 - 목표위치 절댓값 
	int diffX = std::abs(currentPosition.x - goalPosition.x);
	int diffY = std::abs(currentPosition.y - goalPosition.y);

	// 남은 직선 거리 
	int straightDistance = (diffX < diffY) ? diffY : diffX;

	const float straightCost = 1.0f;

	return straightDistance * straightCost;
}

bool AStar::IsInRange(int x, int y, std::vector<std::pair<Craft::Vector2, int>>& grid)
{
	auto it = std::find(grid.begin(), grid.end(), std::make_pair(Vector2(x, y), 1));
	if (it != grid.end()) return true;
	else return false;
}

Node* AStar::FindOpenNode(int x, int y) const
{
	for (Node* node : openList)
	{
		if (node->position == Vector2(x, y))
			return node;
	}
	return nullptr;
}

bool AStar::IsInClosedList(int x, int y) const
{
	for (Node* node : closedList)
	{
		if (node->position == Vector2(x, y))
			return true;
	}
	return false;
}

bool AStar::IsDestination(const Node* node) const
{
	// 비교 노드 다 null아니고 좌표같을떄
	return node && goalNode && node->position == goalNode->position;
}

void AStar::Clearvisualization(std::vector<std::pair<Craft::Vector2, int>>& grid) const
{
	for (int i = 0; i < grid.size(); i++)
	{
		if (grid[i].second == 2)
		{
			grid[i].second = 1;
		}
	}
}

void AStar::DisplayGrid(const std::vector<std::pair<Craft::Vector2, int>>& grid) const
{// 몬스터의 좌표: 시작점, 플레이어 좌표: 목표지점. 
	// 
	for (int i = 0; i < (int)grid.size(); i++)
	{
		if (grid[i].second == 0)
			continue;
		else if(grid[i].second == 2)
			Renderer::Get().Submit(L"+", grid[i].first, Color::PURPLE);
	}
}

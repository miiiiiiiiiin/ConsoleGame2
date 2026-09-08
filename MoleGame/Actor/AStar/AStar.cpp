#include "AStar.h"
#include <algorithm>
#include <iostream>
#include <cstdint>

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

bool AStar::FindPath(
	const Craft::Vector2& startPosition,
	const Craft::Vector2& goalPosition,
	std::unordered_map<int64_t, int>& grid,
	std::vector<Craft::Vector2>& findPosition)
{
	Clear();
	findPosition.clear();

	// 시작 / 목표 위치가 맵에 존재하고 이동 가능한지 확인
	if (!IsInRange(startPosition.x, startPosition.y, grid)
		|| !IsInRange(goalPosition.x, goalPosition.y, grid))
	{
		return false;
	}

	// 이전 탐색 과정의 시각화 값 제거
	Clearvisualization(grid);

	startNode = CreateNode(startPosition);
	goalNode = CreateNode(goalPosition);

	startNode->gCost = 0.0f;
	startNode->hCost = CalculateHeuristic(
		startPosition,
		goalPosition);

	startNode->fCost =
		startNode->gCost + startNode->hCost;

	openList.emplace_back(startNode);

	// 상 / 하 / 좌 / 우
	const std::vector<Direction> directions =
	{
		{ 0, -1, 1.0f },
		{ 0,  1, 1.0f },
		{-1,  0, 1.0f },
		{ 1,  0, 1.0f },
	};

	// OpenList가 빌 때까지 탐색
	while (!openList.empty())
	{
		// FCost가 가장 작은 노드 찾기
		Node* currentNode = openList[0];

		for (Node* node : openList)
		{
			if (node->fCost < currentNode->fCost
				|| (node->fCost == currentNode->fCost
					&& node->hCost < currentNode->hCost))
			{
				currentNode = node;
			}
		}

		// 목적지 도착
		if (IsDestination(currentNode))
		{
			return ConstructPath(
				currentNode,
				findPosition);
		}

		// OpenList에서 제거
		auto openIt = std::find(
			openList.begin(),
			openList.end(),
			currentNode);

		if (openIt != openList.end())
		{
			openList.erase(openIt);
		}

		// ClosedList에 추가
		closedList.emplace_back(currentNode);

		// 현재 위치 기준 4방향 탐색
		for (const Direction& direction : directions)
		{
			int newX =
				currentNode->position.x + direction.x;

			int newY =
				currentNode->position.y + direction.y;

			// 맵에 존재하지 않거나 이동할 수 없는 위치
			if (!IsInRange(newX, newY, grid))
				continue;

			// 이미 방문한 노드라면 건너뜀
			if (IsInClosedList(newX, newY))
				continue;

			// 새로운 GCost
			float newgCost =
				currentNode->gCost + direction.cost;

			// OpenList에 이미 존재하는지 확인
			Node* openNode =
				FindOpenNode(newX, newY);

			if (openNode)
			{
				// 더 짧은 경로를 발견했다면 갱신
				if (newgCost < openNode->gCost)
				{
					openNode->gCost = newgCost;

					openNode->fCost =
						openNode->gCost +
						openNode->hCost;

					openNode->parentNode = currentNode;
				}

				continue;
			}

			// 새로운 노드 생성
			Node* neighborNode =
				CreateNode(
					Vector2(newX, newY),
					currentNode);

			neighborNode->gCost = newgCost;

			neighborNode->hCost =
				CalculateHeuristic(
					neighborNode->position,
					goalNode->position);

			neighborNode->fCost =
				neighborNode->gCost +
				neighborNode->hCost;

			openList.emplace_back(neighborNode);

			// 방문 표시
			auto gridIt =
				grid.find(EncodePos(newX, newY));

			if (gridIt != grid.end())
			{
				// 2 = 방문함
				gridIt->second = 2;
			}
		}
	}

	return false;
}

void AStar::DisplayGridWithPath(
	std::unordered_map<int64_t, int>& grid,
	const std::vector<Craft::Vector2> path,
	Craft::Vector2 cameraPosition,
	const Craft::Vector2 startPosition,
	const Craft::Vector2 goalPosition)
{
	// 기존 탐색 시각화 값 복구
	Clearvisualization(grid);

	// 경로 그리기
	for (const Vector2& position : path)
	{
		// 시작점과 목표점은 제외
		if (position != startPosition
			&& position != goalPosition)
		{
			Renderer::Get().Submit(
				L"*",
				position - cameraPosition,
				Color::PURPLE,
				7);
		}
	}
}

void AStar::Clear()
{
	for (Node* node : allocatedNodes)
	{
		delete node;
	}

	allocatedNodes.clear();
	openList.clear();
	closedList.clear();

	startNode = nullptr;
	goalNode = nullptr;
}

Node* AStar::CreateNode(
	const Craft::Vector2& position,
	Node* parentNode)
{
	Node* node =
		new Node(position, parentNode);

	allocatedNodes.emplace_back(node);

	return node;
}

bool AStar::ConstructPath(
	Node* detinationNode,
	std::vector<Craft::Vector2>& findPosition)
{
	if (!detinationNode)
		return false;

	// 기존 경로가 있다면 제거
	findPosition.clear();

	// 목표 노드부터 부모 노드를 따라 역추적
	Node* current = detinationNode;

	while (current)
	{
		findPosition.emplace_back(
			current->position);

		current = current->parentNode;
	}

	// 역순으로 저장됐으므로 뒤집기
	std::reverse(
		findPosition.begin(),
		findPosition.end());

	return true;
}

float AStar::CalculateHeuristic(
	const Craft::Vector2& currentPosition,
	const Craft::Vector2& goalPosition) const
{
	int diffX =
		std::abs(
			currentPosition.x -
			goalPosition.x);

	int diffY =
		std::abs(
			currentPosition.y -
			goalPosition.y);

	// 상하좌우 이동이므로 Manhattan Distance 사용
	int straightDistance =
		diffX + diffY;

	const float straightCost = 1.0f;

	return straightDistance * straightCost;
}

bool AStar::IsInRange(
	int x,
	int y,
	std::unordered_map<int64_t, int>& grid)
{
	auto it =
		grid.find(EncodePos(x, y));

	// key가 존재하고
	// value가 0이 아니면 이동 가능
	return it != grid.end()
		&& it->second != 0;
}

Node* AStar::FindOpenNode(
	int x,
	int y) const
{
	for (Node* node : openList)
	{
		if (node->position == Vector2(x, y))
		{
			return node;
		}
	}

	return nullptr;
}

bool AStar::IsInClosedList(
	int x,
	int y) const
{
	for (Node* node : closedList)
	{
		if (node->position == Vector2(x, y))
		{
			return true;
		}
	}

	return false;
}

bool AStar::IsDestination(
	const Node* node) const
{
	return node
		&& goalNode
		&& node->position == goalNode->position;
}

void AStar::Clearvisualization(
	std::unordered_map<int64_t, int>& grid) const
{
	for (auto& [key, value] : grid)
	{
		if (value == 2)
		{
			// 2 = 방문함
			// 1 = 이동 가능
			value = 1;
		}
	}
}

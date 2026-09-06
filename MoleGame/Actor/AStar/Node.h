#pragma once
#include <Engine/Engine.h>

class Node
{
public:
	Node(const Craft::Vector2& position, Node* parentNode = nullptr)
		: position(position), gCost(0.0f), hCost(0.0f), fCost(0.0f), parentNode(parentNode)
	{
	}

private:
	Craft::Vector2 position;
	float gCost = 0;
	float hCost = 0;
	float fCost = 0;

	// 최종경로를 역추적하기 위한 부모 노드
	Node* parentNode;
};
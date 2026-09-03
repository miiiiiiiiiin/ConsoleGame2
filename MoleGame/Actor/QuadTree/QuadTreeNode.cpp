#include "QuadTreeNode.h"

QuadTreeNode::QuadTreeNode(const bound& bounds, int capacity)
	: bounds(bounds)
{
}

bool QuadTreeNode::Insert(std::shared_ptr<Craft::Actor> actor)
{
	// 추가: 현재 노드가 가득 찼는지 확인하고 현재 노드에 추가, 자리 없으면 리턴.
	// 정해진 액터의 위치가 현재 노드 바운드 안에 포함되지 않으면 리턴
	if (!bounds.Contains(actor->GetPosition()))
		return false;

	// 아직 분할 안 됐고 여유 있으면 현재 노드에 추가
	if (!isDivided && actors.size() < capacity)
	{
		actors.emplace_back(actor);
		return true;
	}

	// 분할 안 됏고 자리 없으면 4분면으로 쪼갠다.
	// 즉 용량 초과된 노드인 경우 
	if (!isDivided)
		subDivide();
	
	// 분할됐고 나눌 곳 있으면 재귀적으로 삽입.
	// 자식이 true를 반환햇으면 부모도 true를 반환한다!!
	if (topleft->Insert(actor)) return true;
	if (topright->Insert(actor)) return true;
	if (bottomleft->Insert(actor)) return true;
	if (bottomright->Insert(actor)) return true;

	return false;
}

void QuadTreeNode::Query(const bound& range, std::vector<std::shared_ptr<Craft::Actor>>& result) const
{
	// 근처 블록만 찾아보는 질의함수..
	// 노드 영역이 검색 범위랑 아예 안 겹치면 종료.
	if (!bounds.Intersects(range)) return;
	// 위치 겹치면 현재 노드의 리스트에 액터추가
	// 현재 노드에 있는 액터들을 검사.
	for (const auto& actor : actors)
	{
		if (range.Contains(actor->GetPosition()))
		{
			result.emplace_back(actor);
		}
	}

	// 분할되었다면 파고 파고 들어간다 재귀로
	if (isDivided)
	{
		topleft->Query(range, result);
		topright->Query(range, result);
		bottomleft->Query(range, result);
		bottomright->Query(range, result);
	}
	
}

void QuadTreeNode::subDivide()
{
	// 이미 분할되었다면 리턴
	if (isDivided)
		return;

	// 분할.
	int x = bounds.GetX();
	int y = bounds.GetY();
	int halfWidth = bounds.GetWidth() / 2;
	int halfHeight = bounds.GetHeight() / 2;

	topleft = new QuadTreeNode(bound({x, y}, halfWidth, halfHeight), capacity / 2);
	topright = new QuadTreeNode(bound({x + halfWidth, y}, halfWidth, halfHeight), capacity / 2);
	bottomleft = new QuadTreeNode(bound({x, y + halfHeight}, halfWidth, halfHeight), capacity / 2);
	bottomright = new QuadTreeNode(bound({x + halfWidth, y + halfHeight}, halfWidth, halfHeight), capacity / 2);

	isDivided = true;
}

#include "QuadTreeNode.h"
#include <iostream>
#include <Actor/Bomb.h>
#include <Render/Renderer.h>

QuadTreeNode::QuadTreeNode(const bound& bounds, int capacity, int depth)
	: bounds(bounds), capacity(capacity), depth(depth)
{
}

bool QuadTreeNode::Insert(std::shared_ptr<Craft::Actor>& actor, const Craft::Vector2 Bombposition)
{
	// 추가: 현재 노드가 가득 찼는지 확인하고 현재 노드에 추가, 자리 없으면 리턴.
	// 정해진 액터의 위치가 현재 노드 바운드 안에 포함되지 않으면 리턴
	if (!bounds.Contains(actor->GetPosition()))
		return false;

	// 폭탄 있는 위치라면 1번 더 사분면 쪼개기
	if (actor->GetPosition() == Bombposition && !isDivided)
	{
		subDivide();
		if (!isDivided)
		{
			actors.emplace_back(actor);
			return true;
		}
			// 기존 actors 재배치
			for (auto& oldActor : actors)
			{
				if (topleft->Insert(oldActor, Bombposition)) continue;
				if (topright->Insert(oldActor, Bombposition)) continue;
				if (bottomleft->Insert(oldActor, Bombposition)) continue;
				if (bottomright->Insert(oldActor, Bombposition)) continue;
			}
			actors.clear();
	}
	// 아직 분할 안 됐고 여유 있으면 현재 노드에 추가
	if (!isDivided && actors.size() < capacity)
	{
		actors.emplace_back(actor);
		return true;
	}

	// 분할 안 됏고 자리 없으면 4분면으로 쪼갠다.
	// 즉 용량 초과된 노드인 경우 
	if (!isDivided && depth == 0)
	{
		subDivide();
		if (!isDivided)
		{
			actors.emplace_back(actor);
			return true;
		}
		for (auto& actor : actors)
		{
			// 액터 타입이 Bomb이고 depth 1이면 쪼
			if (topleft->Insert(actor, Bombposition)) continue;
			if (topright->Insert(actor, Bombposition)) continue;
			if (bottomleft->Insert(actor, Bombposition)) continue;
			if (bottomright->Insert(actor, Bombposition)) continue;
		}

		actors.clear();
	}

	// ★ 분할되지 않았다면 여기서 끝
	if (!isDivided)
	{
		actors.emplace_back(actor);
		return true;
	}
	// 분할됐고 나눌 곳 있으면 재귀적으로 삽입.
	// 자식이 true를 반환햇으면 부모도 true를 반환한다!!
	if (topleft->Insert(actor, Bombposition)) return true;
	if (topright->Insert(actor, Bombposition)) return true;
	if (bottomleft->Insert(actor, Bombposition)) return true;
	if (bottomright->Insert(actor, Bombposition)) return true;

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
		if (actor && bounds.Contains(range))
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

QuadTreeNode* QuadTreeNode::FindActor(const bound& targeBbounds)
{
	// 겹치지도 않으면 걍 안봄
	if (!bounds.Contains(targeBbounds)) return nullptr;

	// 리프노드면 바운드 리턴
	if (!isDivided)
		return this;

	// 리턴안됏으면 맞는곳 찾아서 내려간다
	if (topleft->bounds.Contains(targeBbounds))
	{
		QuadTreeNode* result = topleft->FindActor(targeBbounds);
		if (result) return result;
	} // null 검사를안해서 오류남
	if (topright->bounds.Contains(targeBbounds))
	{
		QuadTreeNode* result = topright->FindActor(targeBbounds);
		if (result) return result;
	}
	if (bottomleft->bounds.Contains(targeBbounds))
	{
		QuadTreeNode* result = bottomleft->FindActor(targeBbounds);
		if (result) return result;
	}
	if (bottomright->bounds.Contains(targeBbounds))
	{
		QuadTreeNode* result = bottomright->FindActor(targeBbounds);
		if (result) return result;
	}

	return nullptr;
}

void QuadTreeNode::DrawingQuadTree(const Craft::Vector2 position)
{
	
	int x = bounds.GetX() - position.x;
	int y = bounds.GetY() - position.y;
	int width = bounds.GetWidth();
	int height = bounds.GetHeight();
	// 좌 / 우
	for (int i = 0; i < height; i++)
	{
		Renderer::Get().Submit(L"|", Vector2(x, y + i), Color::GREEN, 6);
		Renderer::Get().Submit(L"|", Vector2(x + width - 1, y + i), Color::GREEN, 6);
	}
	// 상 / 하
	for (int i = 0; i < width; i++)
	{
		if (i == 0)
		{
			Renderer::Get().Submit(L"┌", Vector2(x + i, y), Color::GREEN, 6);
			Renderer::Get().Submit(L"└", Vector2(x + i, y + height - 1), Color::GREEN, 6);
		}
		else if (i == width - 1)
		{
			Renderer::Get().Submit(L"┐", Vector2(x + i, y), Color::GREEN, 6);
			Renderer::Get().Submit(L"┘", Vector2(x + i, y + height - 1), Color::GREEN, 6);
		}
		else
		{
			Renderer::Get().Submit(L"-", Vector2(x + i, y), Color::GREEN, 6);
			Renderer::Get().Submit(L"-", Vector2(x + i, y + height - 1), Color::GREEN, 6);
		}

	}


	// 재귀적으로 자식 노드 바운딩 박스 그리기
	if (isDivided)
	{
		if(topleft) topleft->DrawingQuadTree(position);
		if(topright) topright->DrawingQuadTree(position);
		if(bottomleft) bottomleft->DrawingQuadTree(position);
		if(bottomright) bottomright->DrawingQuadTree(position);
	}

}

void QuadTreeNode::subDivide()
{
	if (depth > 1) return;
	// 이미 분할되었다면 리턴
	if (isDivided)
		return;

	// 분할.
	int x = bounds.GetX();
	int y = bounds.GetY();
	int halfLeftWidth = bounds.GetWidth() / 2;
	int halfRightWidth = bounds.GetWidth() - halfLeftWidth;
	int halfTopHeight = bounds.GetHeight() / 2;
	int halfBottomHeight = bounds.GetHeight() - halfTopHeight;

	topleft = new QuadTreeNode(bound(Craft::Vector2(x, y), halfLeftWidth, halfTopHeight), capacity, depth + 1);
	topright = new QuadTreeNode(bound(Craft::Vector2(x + halfLeftWidth, y), halfRightWidth, halfTopHeight), capacity, depth + 1);
	bottomleft = new QuadTreeNode(bound(Craft::Vector2(x, y + halfTopHeight), halfLeftWidth, halfBottomHeight), capacity, depth + 1);
	bottomright = new QuadTreeNode(bound(Craft::Vector2(x + halfLeftWidth, y + halfTopHeight), halfRightWidth, halfBottomHeight), capacity, depth + 1);
	//depthCount++;
	isDivided = true;
	std::cout
		<< "parent H: " << bounds.GetHeight()
		<< " / "
		<< halfTopHeight
		<< " + "
		<< halfBottomHeight
		<< '\n';
	std::cout
		<< "TL H: " << topleft->bounds.GetHeight()
		<< " TR H: " << topright->bounds.GetHeight()
		<< " BL H: " << bottomleft->bounds.GetHeight()
		<< " BR H: " << bottomright->bounds.GetHeight()
		<< '\n';
}

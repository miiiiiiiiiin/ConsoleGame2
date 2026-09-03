#include "QuadTreeNode.h"

QuadTreeNode::QuadTreeNode(const bound& bounds, int depth)
	: bounds(bounds), depth(depth)
{
}

QuadTreeNode::~QuadTreeNode()
{
	Clear();
}

void QuadTreeNode::Clear()
{
	points.clear();
	ClearChildren();
}

// 맵 로딩시 액터 리스트에 추가된 액터들만 추가해본다..
bool QuadTreeNode::Insert(std::shared_ptr<Actor>& actor)
{
	if (!actor) return false;
	// 현재 영역에 포함되지 않으면 삽입처리 안함
	if (!bounds.Contains(actor->GetPosition()))
		return false;

	bound newBound(Vector2(actor->GetPosition().x, actor->GetPosition().y));
	// 객체가 삽입될 사분면 확인
	NodeIndex result = TestRegion(newBound);

	// 여러 사분면에 걸쳐있거나 더이상 나눌 수 없ㄴ으면 현재 노드 저장
	if (result == NodeIndex::Straddling || !SubDivide())
	{
		points.emplace_back(actor);
		return true;
	}

	// 더 들어갈 수 있는지 재귀적으로 확인
	if (result == NodeIndex::TopLeft)
		return topLeft->Insert(actor);
	if (result == NodeIndex::TopRight)
		return topRight->Insert(actor);
	if (result == NodeIndex::BottomLeft)
		return bottomLeft->Insert(actor);
	if (result == NodeIndex::BottomRight)
		return bottomRight->Insert(actor);

	return false;
}
QuadTreeNode* QuadTreeNode::FindLeafNodeQuery(Vector2& position)
{
	// 범위검사: 현재 노드에 포함되지 않으면 종료
	if (!bounds.Contains(position))
		return nullptr;

	// 더이상 안 쪼개지면 반환
	if (!IsDivided())
		return this;

	// 나눠진다면 재귀로 ㄱㄱ
	if (QuadTreeNode* found = topLeft->FindLeafNodeQuery(position)) return found;
	if (QuadTreeNode* found = topRight->FindLeafNodeQuery(position)) return found;
	if (QuadTreeNode* found = bottomLeft->FindLeafNodeQuery(position)) return found;
	if (QuadTreeNode* found = bottomRight->FindLeafNodeQuery(position)) return found;

	return nullptr;
}
// 바운드 영역: 카메라 영역 / 검출된 노드 담을 액터 배열.
void QuadTreeNode::Query(const bound& queryBounds, std::vector<std::shared_ptr<Actor>>& results) const
{
	// 검사 영역과 겹치지 않으면 더이상 검사 x
	if (!bounds.Intersects(queryBounds))
		return;

	// 현재 노드에 저장된 객체를 실제 영역과 비교
	for (std::shared_ptr<Actor> actor : points)
	{
		if (queryBounds.Contains(actor->GetPosition()))
		{
			results.emplace_back(actor);
		}
	}

	// 분할된 경우라면 자식 노드까지 검사
	if (IsDivided())
	{
		topLeft->Query(queryBounds, results);
		topRight->Query(queryBounds, results);
		bottomLeft->Query(queryBounds, results);
		bottomRight->Query(queryBounds, results);
	}
}



bool QuadTreeNode::SubDivide()
{
	// 예외처리
	// 최대 깊이 넘었으면 x
	if (depth >= maxDepth) return false;

	// 분할되었으면 반환
	if (IsDivided()) return true;

	// 분할 시작
	int x = bounds.GetX();
	int y = bounds.GetY();
	int halfWidth = bounds.GetWidth() / 2;
	int halfHeight = bounds.GetHeight() / 2;

	topLeft = new QuadTreeNode(bound(Vector2(x, y), halfWidth, halfHeight), depth + 1);
	topRight = new QuadTreeNode(bound(Vector2(x + halfWidth, y), halfWidth, halfHeight), depth + 1);
	bottomLeft = new QuadTreeNode(bound(Vector2(x, y+ halfHeight), halfWidth, halfHeight), depth + 1);
	bottomRight = new QuadTreeNode(bound(Vector2(x + halfWidth, y+ halfHeight), halfWidth, halfHeight), depth + 1);

	return false;
}

bool QuadTreeNode::IsDivided() const
{
	// 자손 노드 중 1개라도 nullptr이 아니라면 분할되었다고 판단
	// -> SubDivide를 거쳤는가..
	
	return !!topLeft;
}

QuadTreeNode::NodeIndex QuadTreeNode::TestRegion(const bound& targetBounds) const
{
	// 현재 노드의 영역을 벗어난 경우 확인
	if (!bounds.Contains(targetBounds))
		return NodeIndex::OutOfArea;

	// 전달된 영역이 어느 사분면에 포함되는지 확인
	std::vector<QuadTreeNode::NodeIndex> quads = GetQuads(targetBounds);

	// 반환받은 배열의 요소 개수가 1개라면, 4분면 중 1개 영역ㅇ[ 완전 포함
	if (quads.size() == 1) return quads[0];

	// 여러 영역에 겹침
	return NodeIndex::Straddling;
}

std::vector<QuadTreeNode::NodeIndex> QuadTreeNode::GetQuads(const bound& targetBounds) const
{
	// 겹치는 영역을 포함할 배열
	std::vector<NodeIndex> quads;

	// 영역 계산에 필요한 변수
	int x = bounds.GetX();
	int y = bounds.GetY();
	int halfWidth = bounds.GetWidth() / 2;
	int halfHeight = bounds.GetHeight() / 2;
	int centerX = x + halfWidth;
	int centerY = y + halfHeight;
	// 왼쪽 영역에 완전히 포함되는지 확인
	bool left = targetBounds.GetX() >= x && targetBounds.GetXMax() <= centerX;
	// 오른쪽 영역에 완전히 포함되는지 확인
	bool right = targetBounds.GetX() >= centerX && targetBounds.GetXMax() <= bounds.GetXMax();
	bool top = targetBounds.GetY() >= y && targetBounds.GetYMax() <= centerY;
	bool bottom = targetBounds.GetY() >= centerY && targetBounds.GetYMax() <= bounds.GetYMax();

	if (top && left)
		quads.emplace_back(NodeIndex::TopLeft);
	if (top && right)
		quads.emplace_back(NodeIndex::TopRight);
	if (bottom && left)
		quads.emplace_back(NodeIndex::BottomLeft);
	if (bottom && right)
		quads.emplace_back(NodeIndex::BottomRight);

	return quads;
}

void QuadTreeNode::ClearChildren()
{
	if (IsDivided())
	{
		SafeDelete(topLeft);
		SafeDelete(topRight);
		SafeDelete(bottomLeft);
		SafeDelete(bottomRight);
	}
}

#pragma once
#include "bound.h"
#include <vector>
template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

// 4등분된 노드 클래스
class QuadTreeNode: public Craft::Level
{
	enum class NodeIndex
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Straddling, // 경계선 걸치는 경우
		OutOfArea // 영역 벗어난 경우
	};
public:
	QuadTreeNode(const bound& bounds, int depth = 0);
	~QuadTreeNode();

	void Clear();

	// 노드에 삽입할 때 사용할 함수
	bool Insert(std::shared_ptr<Actor>& actor);

	// 검색 영역과 실제로 겹치는 노드 목록 반환
	void Query(const bound& queryBounds, std::vector<std::shared_ptr<Actor>>& results) const;

	// 게터
	const bound& GetBounds() const { return bounds; }
	const std::vector<std::shared_ptr<Actor>>& GetPoints() const { return points; } // 현재 노드에 포함되 ㄴ영역
	QuadTreeNode* TopLeft() const { return topLeft; }
	QuadTreeNode* TopRight() const { return topRight; }
	QuadTreeNode* BottomLeft() const { return bottomLeft; }
	QuadTreeNode* BottomRight() const { return bottomRight; }

	// 현재 폭탄이 설정된 위치를 갖다주면 그 위치가 어느 리프노드인지 확인 후 액터리스트에 담는다
	QuadTreeNode* FindLeafNodeQuery(Vector2& position);

private:
	// 영역을 4분할하는 함수. 최대깊이 도달 시 false
	bool SubDivide();

	// 영역이 분할되었는지 확인하는 함수
	bool IsDivided() const;

	// 전달한 영역과 겹치는 영역을 반환하는 함수
	NodeIndex TestRegion(const bound& targetBounds) const;

	//  전달 영역이 포함되거나 걸치는 영역을 모두 구할 때 사용 함수
	std::vector<NodeIndex> GetQuads(const bound& targetBounds) const;

	// 자식 노드 정리 함수
	void ClearChildren();



private:
	// 현재 노드 깊이
	int depth = 0;

	// 최대 깊이
	int maxDepth = 3;

	// 현재 노드의 영역
	bound bounds;

	// 현재 영역에 포함된 노드
	std::vector<std::shared_ptr<Actor>> points;

	// 자식 노드
	QuadTreeNode* topLeft = nullptr;
	QuadTreeNode* topRight = nullptr;
	QuadTreeNode* bottomLeft = nullptr;
	QuadTreeNode* bottomRight = nullptr;

};
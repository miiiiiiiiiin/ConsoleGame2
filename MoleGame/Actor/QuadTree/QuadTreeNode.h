#include "bound.h"
#include <vector>
#include <Actor/Actor.h>

class QuadTreeNode
{
	enum class NodeIndex
	{
		// 경계선에 걸치지 않고 완전 포함
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Straddling, // 경계선에 걸치는 경우
		OutOfArea // 영역을 벗어난 경우
	};
public:
	QuadTreeNode(const bound& bounds, int capacity = 9, int depth = 0);
	// 추가함수
	bool Insert(std::shared_ptr<Craft::Actor>& actor, const Craft::Vector2 Bombposition);

	// 질의함수
	void Query(const bound& range, std::vector<std::shared_ptr<Craft::Actor>>& result) const;

	// 특정 노드 찾기
	QuadTreeNode* FindActor(const bound& bounds);

	// 디버그모드호출시 화면에 쿼드트리 그리기
	void DrawingQuadTree(const Craft::Vector2 position);

	std::vector<std::shared_ptr<Craft::Actor>>& Getactors() { return actors; }


private:
	// 4개로 분할하는 함수
	void subDivide();
	NodeIndex TestRegion(const bound& targetBounds) const;
	std::vector<QuadTreeNode::NodeIndex> GetQuads(const bound& targetBounds) const;
private:
	bound bounds;

	// 한 노드가 분할 없이 담을 수 있는 액터 개수
	int capacity = 30;
	// 노드에 담긴 액터들
	std::vector<std::shared_ptr<Craft::Actor>> actors;
	QuadTreeNode* topleft = nullptr;
	QuadTreeNode* topright = nullptr;
	QuadTreeNode* bottomleft = nullptr;
	QuadTreeNode* bottomright = nullptr;

	// 쪼개졋는지 확인
	bool isDivided = false;

	int depth = 2;
	int depthCount = 0;


	// 
};
#include "bound.h"
#include <vector>
#include <Actor/Actor.h>

class QuadTreeNode
{
public:
	QuadTreeNode(const bound& bounds, int capacity = 38);
	// 추가함수
	bool Insert(std::shared_ptr<Craft::Actor> actor);

	// 질의함수
	void Query(const bound& range, std::vector<std::shared_ptr<Craft::Actor>>& result) const;
private:
	// 4개로 분할하는 함수
	void subDivide();
private:
	bound bounds;

	// 한 노드가 분할 없이 담을 수 있는 액터 개수
	int capacity;
	// 노드에 담긴 액터들
	std::vector<std::shared_ptr<Craft::Actor>> actors;
	QuadTreeNode* topleft = nullptr;
	QuadTreeNode* topright = nullptr;
	QuadTreeNode* bottomleft = nullptr;
	QuadTreeNode* bottomright = nullptr;

	// 쪼개졋는지 확인
	bool isDivided = false;


};
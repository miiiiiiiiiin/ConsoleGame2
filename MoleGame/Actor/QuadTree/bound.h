#pragma once
#include <Level/Level.h>

using namespace Craft;
class bound 
{
public:
	bound(Vector2 position, int width = 1, int height = 1)
		: bounds(position), width(width), height(height)
	{
		
	}

	//게터
	int GetX() const { return bounds.x; }
	int GetY() const { return bounds.y; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	int GetXMax() const { return bounds.x + width; }
	int GetYMax() const { return bounds.y + height; }

	// 검사 버텍스가 영역 안에 포함되는지 확인
	bool Contains(Vector2& otherPos) const
	{
		return (otherPos.x >= bounds.x && otherPos.x < GetXMax())
			&& (otherPos.y >= bounds.y && otherPos.y < GetYMax());
	}

	// 다른 사각형이 현재 영역에 포함되는지 확인
	bool Contains(const bound& other) const
	{
		return other.bounds.x >= bounds.x && other.GetXMax() <= GetXMax()
			&& other.bounds.y >= bounds.y && other.GetYMax() <= GetYMax();
	}

	// 다른 사각형과 겹치는지 확인하는 함수
	bool Intersects(const bound& other) const
	{
		return other.bounds.x < GetXMax() && other.GetXMax() > bounds.x
			&& other.bounds.y < GetYMax() && other.GetYMax() > bounds.y;
	}



private:
	// 시작 버텍스
	Vector2 bounds;
	// 가로세로 길이
	int width = 1;
	int height = 1;
};
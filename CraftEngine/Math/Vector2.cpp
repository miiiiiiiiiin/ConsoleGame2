#include"Vector2.h"
#include<assert.h>

namespace Craft
{
	Vector2 Vector2::Zero(0, 0);
	Vector2 Vector2::One(1, 1);
	Vector2 Vector2::Right(1, 0);
	Vector2 Vector2::Up(0, -1);

	Vector2::Vector2(int x, int y) : x(x), y(y)
	{
	}
	Vector2::operator COORD() const
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord; // Vector2 를 coord 타입으로 변경
	}
	Vector2::operator COORD()
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord; // Vector2 를 coord 타입으로 변경
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{

		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator*(const Vector2& other) const
	{
		return Vector2(x * other.x, y * other.y);
	}
	Vector2 Vector2::operator/(const Vector2& other) const
	{
		//어서트(릴리즈시삭제): 분모값이 0이면 무한대 방지
		assert(other.x != 0 && other.y != 0);
		return Vector2(x / other.x, y / other.y);
	}
	Vector2& Vector2::operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}
	bool Vector2::operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}
	bool Vector2::operator!=(const Vector2& other) const
	{
		return (x != other.x) || (y != other.y);
		//return !(*this == other); 같은말임(난뭔말인지몰르겟슴)..
	}
}
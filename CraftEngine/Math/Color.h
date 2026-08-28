#pragma once
#include<Core/Core.h>
#include<Windows.h>

namespace Craft
{
	//색상을 열거형으로 정의
	enum class CRAFT_API Color : WORD
	{
		BLUE = FOREGROUND_BLUE,
		GREEN = FOREGROUND_GREEN,
		RED = FOREGROUND_RED,
		YELLOW = RED | GREEN,
		CYAN = GREEN | BLUE,
		PURPLE = RED | BLUE,
		WHITE = RED | GREEN | BLUE,
		BRIGHTWHITE = WHITE | FOREGROUND_INTENSITY,
		CLASSICGREEN = 0x0A,
		brightBlue = 46,
		SKY = 11
	};
}

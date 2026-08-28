#include "ScreenBuffer.h"
#include<cassert>
#include<iostream>

namespace Craft
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
		: size(screenSize)
	{
		//콘솔 버퍼 생성: CreateConsoleScreenBuffer()
		buffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr, // 상속x
			CONSOLE_TEXTMODE_BUFFER,
			NULL
		);

		//값 확인: INVALID_HANDLE_VALUE 반환되면 실패한거임
		assert(buffer != INVALID_HANDLE_VALUE);

		//화면 창 크기 설정
		SMALL_RECT rect = {};
		rect.Top = 0;
		rect.Left = 0;
		rect.Right = static_cast<short>(size.x - 1);
		rect.Bottom = static_cast<short>(size.y - 1);
		//SetConsoleWindowInfo함수: 콘솔 창 크기 설정함(오른.아래가 버퍼보다 1 작게)
		BOOL result = SetConsoleWindowInfo(buffer, TRUE, &rect);

		//결과 확인
		assert(result == TRUE);
		
		//화면 버퍼 크기 설정
		SetConsoleScreenBufferSize(buffer, size);
		assert(result == TRUE);

		//직접 만든 콘솔의 커서 끄기
		CONSOLE_CURSOR_INFO info;
		result = GetConsoleCursorInfo(buffer, &info);
		assert(result == TRUE);

		//커서 안보이게 설정
		info.bVisible = FALSE;
		result = SetConsoleCursorInfo(buffer, &info);
		assert(result == TRUE);


	}

	ScreenBuffer::~ScreenBuffer()
	{
		//콘솔 닫기 해야댐
		if (buffer)
			CloseHandle(buffer);
	}

	void ScreenBuffer::Clear() const
	{
		//콘솔 화면 전체 지우는 함수
		//공백 문자를 화면 전체에 한  번에 설정

		//화면에 설정된 글자 수
		DWORD writtenCount = 0;

		BOOL result = FillConsoleOutputCharacterW(
			buffer,
			' ', // 공백으로 다 지워진거처럼
			size.x * size.y, //요만큼
			Vector2::Zero, // 0,0부터
			&writtenCount // 반환값 변수

		);
		assert(result == TRUE);

	}

	void ScreenBuffer::Draw(const CHAR_INFO* const charInfo) const
	{//CHAR_INFO: 2차원배열(1차원 배열에 2차원 배열 정보를 기록함)
		//콘솔에 CHAR_INFO 타입으로 글자 쓰는 함수
		//설정할 글자 영역
		SMALL_RECT rect = {
			0,							// Left
			0,							// top
			static_cast<short>(size.x - 1), // right
			static_cast<short>(size.y - 1)  // bottom
		};

		BOOL result = WriteConsoleOutputW(
			buffer,
			charInfo, 
			size, // charinfo 배열의 길이
			Vector2::Zero, // 0.0부터
			&rect
		);

		assert(result == TRUE);
	}

}
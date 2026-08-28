#pragma once
#include<Windows.h>
#include<Math/Vector2.h>

namespace Craft 
{
	//이중 버퍼링 구현을 위한 화면 버퍼 클래스
	//콘솔 핸들을 관리
	class ScreenBuffer
	{
	public:
		ScreenBuffer(const Vector2& screenSize);
		~ScreenBuffer();

		//콘솔 초기화
		void Clear() const;

		//전달된 글자 값 그리는 함수
		void Draw(const CHAR_INFO* const charInfo) const;

		//Getter
		inline HANDLE GetBuffer() const { return buffer; }
		//(핸들:콘솔위의 클라이언트창을 핸들이 접근할수잇게해줌)




	private:
		//화면 버퍼 핸들
		HANDLE buffer = nullptr;

		//화면 크기
		Vector2 size;


	};
}



#include<iostream>
#include "Input.h"
#include <assert.h>
#include<Windows.h>


namespace Craft
{

	//static 변수 초기화
	Input* Input::instance = nullptr;

	Input::Input()
	{
		//시작할 때 instance값은 null이어야 함
		assert(!instance && "instance should be null here.");
		instance = this;
	}
	bool Input::GetKeyDown(int keyCode) const
	{
		//이전키 안눌림, 현재키 눌림
		return !keyStates[keyCode].wasKeyDown
			&& keyStates[keyCode].isKeyDown;
	}
	bool Input::GetKeyUp(int keyCode) const
	{
		//이전키 눌림, 현재키 안눌림
		return keyStates[keyCode].wasKeyDown
			&& !keyStates[keyCode].isKeyDown;
	}
	bool Input::GetKey(int keyCode) const
	{
		//현재키 눌림
		return keyStates[keyCode].isKeyDown;
	}
	Input& Input::Get()
	{
		//여기에서 instance값은 null이면 안됨
		assert(instance && "instance should not be null here.");
		return *instance;
	}
	void Input::ProcessInput()
	{
		//현재 프레임에 키 입력이 발생했는지 확인
		for (int i = 0; i < keyCount; i++)
		{
			//키 눌림 저장(특정 키가 눌리면 0x8000: 최상위 비트가 반환된다)
			//GetAsyncKeyState(i) & 0x8000(AND연산)이 1이면 true, 아니면 false 
			keyStates[i].isKeyDown = ((GetAsyncKeyState(i) & 0x8000) != 0);
		}
	}
	void Input::SavePreviousStates()
	{
		//이전 프레임 입력 값 저장
		for (KeyState& state : keyStates)
		{
			//현재프레임->이전프레임에 넣기
			state.wasKeyDown = state.isKeyDown;
		}
	}
}
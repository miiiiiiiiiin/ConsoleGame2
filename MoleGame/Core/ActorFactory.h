#pragma once
#include <unordered_map>
#include <functional>
//#include <Level/Level.h>
using namespace Craft;

// 액터들을 해시테이블로 관리하는 클래스
class ActorFactory
{
public: 
	// 데이터값(second)을 func로 만들기
	using CreateFunc = std::function<void(Level&, const Vector2&)>;

	/* -------------- 공개 메소드 ------------- */
	
	// 특정 문자에 생성 함수를 등록
	static void Register(char key, CreateFunc func)
	{
		GetRegistry()[key] = func;
	}
	static bool Create(char key, Level& level, const Vector2& position)
	{
		// 해시테이블 상태 가져오기
		auto& registry = GetRegistry();
		// key값 있는지 검색
		auto it = registry.find(key);
		// 없으면 종료
		if (it == registry.end())
			return false;
		// 두번째 값 설정하여 액터 생성
		it->second(level, position);
		return true;

	}

private:
	// 한번 호출되면 계속 갖고 잇는 본인의 정보,.
	static std::unordered_map<char, CreateFunc>& GetRegistry()
	{
		std::unordered_map<char, CreateFunc> registry;
		return registry;
	}
};
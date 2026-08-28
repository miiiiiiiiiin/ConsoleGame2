#pragma once
#include <Core/Core.h>
#include <memory>

namespace Craft 
{
	// 커스텀 타입 시스템을 제공하는 최상위 클래스
	// 단일 상속만 지원
	class CRAFT_API CraftObject
	{
	public:
		// 가상 소멸자(상속하니꼐)
		virtual ~CraftObject() = default;

		// 현재 객체 타입 Id 를 반환
		//순수가상함수 -> 상속하는계층 강제 구현
		//c++ 에서인터페이스 사용시 일케구성
		virtual size_t GetType() const = 0;

		// 전달된 다른 타입 id와 현재 객체(부모 포함)
		virtual bool Is(size_t id) const
		{
			// 최상위 객체인 CraftObject까지 접근했다는 것은 타입이 다르다는것을 의미함
			return false;
		}

		//타입 질의 함수. id가져와서 Is함수로 넘김
		//이 함수를 사용하기 위해서는 T타입(클래스)이 static TypeId함수를 가져야만 함
		template<typename T>
		bool IsTypeOf() const
		{
			return Is(T::TypeId());
		}

	};

	// 형변환 함수 - 스마트 포인터 간의 변환 처리
	template<typename T, typename U>
	std::shared_ptr<T> Cast(const std::shared_ptr<U>& object)
	{
		// 예외처리
		if (!object)
		{
			return nullptr;
		}

		// object의 실제 타입이 T(또는 T의 파생 타입)인지 확인 후 형변환(캐스팅)
		if (object->Is(T::TypeId()))
		{
			// 가벼운 형변환을 통해 반환
			// static_pointer_cast: 런타임에 타입 검증을 하지 않아서 상대적으로 가벼움
			return std::static_pointer_cast<T>(object);
		}

		// 타입 검사에 실패하면 null반환
		return nullptr;
	}


}

// 타입 시스템을 사용하는 클래스에 배치할 매크로.
// 반복적인 코드 자동화를 할 때 많이 활용됨.
#define TYPE_DECLARATIONS(Type, ParentType)									\
	using super = ParentType;												\
protected:																	\
	/* 전역 지역 변수의 주소를 활용해 유니크한 id를 반환하는 함수 */		\
	static size_t TypeIdClass()												\
	{																		\
		static int runTimeTypeId = 0;										\
		return reinterpret_cast<size_t>(&runTimeTypeId);					\
	}																		\
public:																		\
	static size_t TypeId()													\
	{																		\
		return Type::TypeIdClass();											\
	}																		\
	virtual size_t GetType() const override									\
	{																		\
		return Type::TypeIdClass();											\
	}																		\
	virtual bool Is(size_t id) const override								\
	{	/* 현재 계층에서 일단 비교하고 타입이 다르면 부모 계층까지 검색*/	\
		return (id == TypeIdClass()) ? true : ParentType::Is(id);			\
	}
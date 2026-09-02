#pragma once
#include<Core/CraftObject.h>
#include<Core/Core.h>
#include<Actor/Actor.h>
#include<memory> // 스마트포인터 사용
#include<vector> 

namespace Craft
{
	// 게임에 배치된 모든 액터를 관리하는 클래스.
	// public std::enable_shared_from_this<Level>
	// : shared_from_this() / weak_from_this() 사용하기 위해.
	// : shared_from_this() - this 포인터를 shared_ptr로 변환.
	// : weak_from_this() - this 포인터를 weak_ptr로 변환.
	class CRAFT_API Level : 
		public CraftObject,
		public std::enable_shared_from_this<Level>
	{
		// 커스텀 타입 설정
		TYPE_DECLARATIONS(Level, CraftObject)

		//friend선언
		friend class Engine;

	public:
		Level();
		virtual ~Level();

		//초기화 함수
		virtual void OnInitialized();

		//게임 플레이 이벤트 함수
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		//액터 추가 함수(템플릿) ...args:가변인자
		//typename ...Args=> 인자를 여러 개 받을 수 있다 라는 뜻
		//std::enable_if_t<std::is_base_of<Actor, T>::value> => SFINAE 라고함
		// T가 Actor를 상속받았으면 true 즉 액터인지 아닌지 판별한다는 뜻
		template<typename T, typename ...Args, 
			typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
			std::shared_ptr<T> SpawnActor(Args&& ...args)
		{
			//새로운 객체 생성
			std::shared_ptr<T> newActor
				= std::make_shared<T>(std::forward<Args>(args)...);

			//추가 요청 목록에 포함
			addRequestedActorList.emplace_back(newActor);

			newActor->SetOwner(weak_from_this());

			//생성한 액터 반환
			return newActor;
		}

		// 특정 위치에 있는 특정 타입의 액터를 찾는 함수.
		template<typename T,
			typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
			std::shared_ptr<T> FindActorAt(const Vector2& position)
		{
			for (const auto& actor : actorList)
			{
				if (actor->GetPosition() != position)
				{
					continue;
				}

				std::shared_ptr<T> target = std::dynamic_pointer_cast<T>(actor);
				if (target)
				{
					return target;
				}
			}
			return nullptr;
		}


		//액터 검색 함수(템플릿)
		template<typename T,
			typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> FindAtor()
		{
			//검색: 형변환
			for (const auto& actor : actorList)
			{
				//T타입으로 형변환 시도
				std::shared_ptr<T> targetActor
					= std::dynamic_pointer_cast<T>(actor);

				if (targetActor) return targetActor;

			}

			//못찾은 경우 null반환
			return nullptr;
		}


		//Getter
		inline bool HasInitialized() const { return hasInitialized; }

		Vector2 GetCameraPosition() const { return cameraPosition; }
		void SetCameraPosition(const Vector2& pos) { cameraPosition = pos; }


	protected:
		//이전 프레임에 추가/제거 요청된 액터 처리 함수
		void ProcessAddAndDestroyActors();

		// 액터의 이전 상태 처리 함수
		void SavePreviousActorStates();

	protected:
		//초기화 처리 여부 플래그
		bool hasInitialized = false;

		//레벨에 배치된 모든 액터
		std::vector<std::shared_ptr<Actor>> actorList;

		//레벨에 추가 요청된 액터를 저장해두는 목록.(예약목록)
		//현재 프레임을 처리하는 과정에서 액터 추가 요청이 발생되면
		//해당 액터를 바로 추가하면 기존 액터 처리에 문제가 발생할 수 있어서 
		//현재 프레임을 모두 처리한 후에 추가 요청된 액터를 actorList로 옮김
		std::vector<std::shared_ptr<Actor>> addRequestedActorList;

		// 카메라 위치 변수
		Vector2 cameraPosition = Vector2(0, 0);

	};
}



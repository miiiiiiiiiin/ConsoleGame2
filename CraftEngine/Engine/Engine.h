#pragma once
#include<memory> // 스마트 포인터 사용
#include<Core/Core.h>
#include<Render/Renderer.h>
#include <string>


class Sound;
namespace Craft
{
	//전방 선언
	class Level;
	class Input;
	class Renderer;
	class CollisionSystem;

	//메인 엔진 클래스
	//엔진 루프 제공, 게임 엔진 핵심기능 제공
	class CRAFT_API Engine
	{
		//엔진설정(데이터)
		struct Setting
		{
			//목표 프레임 수(초당프레임)
			float framerate = 0.0f;

			//사용할 콘솔 화면 너비
			int width = 0;

			//사용할 콘솔 화면 높이
			int height = 0;
		};

	public:
		Engine();
		virtual ~Engine();
		//엔진실행함수
		void Run();
		//엔진종료함수
		void Quit();
		
		// 사운드 재생 함수(사운드 시스템 래퍼 함수
		void PlayOnShot(const std::string& filename);
		void PlayBackgroundMusic(const std::string& filename);
		void StopBackgroundMusic();

		//레벨 추가 요청 함수
		// 1. std::is_base_of 하는일이 무엇인지
		// 2. std::enable_if_t 하는일이 무엇인지
		// 3. typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		template<typename T, 
			typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		void AddNewLevel()
		{
			//추가 요청 레벨 객체 생성
			nextLevel = std::make_shared<T>();
		}

		//전역 접근 함수
		static Engine& Get();
		
		//콘솔 너비 높이 Getter
		inline int GetWidth() const { return setting.width; }
		inline int GetHeight() const { return setting.height; }
		inline int GetRate() const { return setting.framerate; }

	protected:
		//입력 처리 함수(입력 폴링)
		void ProcessInput();

		//프레임 시간 계산
		
		//초기화함수
		void OnInitialized();

		//게임 플레이 이벤트 함수
		//게임 초기화함수
		void BeginPlay();

		//게임 플레이 업데이트 함수
		void Tick(float deltaTime);

		//레벨 그리기 함수
		void Draw();

		// 충돌 처리 함수
		void ProcessCollision();

		//프레임 간 입력 값 저장을 위한 함수
		void SavePriviousInputStates();

		//엔진 종료시 정리 함수
		void ShutDown();

		//엔진 설정 로드 함수
		void LoasEngineSetting();

	protected:
		//엔진 종료 요청 여부 플래그
		bool isQuit = false;

		//엔진 설정 함수
		Setting setting;

		//전역 접근이 가능하도록 변수 선언
		static Engine* instance;

		//메인 레벨
		std::shared_ptr<Level> mainLevel;

		//추가 요청된 레벨
		std::shared_ptr<Level> nextLevel;
		
		//입력 시스템 변수
		std::unique_ptr<Input> input;

		//렌더러
		std::unique_ptr<Renderer> renderer;

		// 콜리전 시스템
		std::unique_ptr<CollisionSystem> collisionSystem;

		// 사운드 시스템 객체
		std::unique_ptr<Sound> sound;
	};
}




#include "Actor/Actor.h"
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Level/Level.h>

namespace Craft
{
	Actor::Actor(
		const std::wstring& image,
		const Vector2& position,
		Color color
	): image(image), position(position), color(color), width(static_cast<int>(image.length()))
	{
	}
	 Actor::~Actor()
	{
	}

	void  Actor::BeginPlay()
	{
		//이벤트 처리했다고 설정
		hasBeganPlay = true;

	}

	void  Actor::Tick(float deltaTime)
	{
	}

	void  Actor::Draw()
	{
		if (!IsActive()) return;
		Vector2 newPosition = position;
		std::shared_ptr<Level> level = GetOwner();
		// HACK: make_shared로 햇는데 충돌 안나는지..
		//std::shared_ptr<Engine> game = std::make_shared<Engine>();
		if (level)
		{
			newPosition = position - level->GetCameraPosition();
		}
		if (newPosition.x < 0 || newPosition.x >= 30
			|| newPosition.y < 0 || newPosition.y >= Engine::Get().GetHeight())
			return;
		//렌더러에 필요한 데이터 제출
		Renderer::Get().Submit(image, newPosition, color, sortingOrder);

	}

	void Actor::OnCollision(const std::shared_ptr<Actor>& other)
	{
	}

	void Actor::Destroy()
	{
		//삭제 예약 설정
		hasExpired = true;
	}

	void Actor::QuitGame()
	{
		//엔진 종료 요청
		Engine::Get().Quit();
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		//변경하려는 위치 값이 기존 값과 동일하면 종료
		if (position == newPosition) return;

		position = newPosition;
	}

}

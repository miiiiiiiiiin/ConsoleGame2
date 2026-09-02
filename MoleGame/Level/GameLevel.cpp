#include <cassert>
#include <algorithm>
#include "GameLevel.h"
#include <Game/Game.h>
#include <Actor/Mole.h>
#include <Actor/Wall.h>
#include <Actor/Dirt.h>

using namespace Craft;
void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	LoadMap("Map.txt");
}

void GameLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
	FPS = 1.0f / deltaTime;
	// 카메라 이동
	UpdateCamera(deltaTime);
}

void GameLevel::UpdateCamera(float deltaTime)
{
	// 프레임 크기 가져오기
	Game& game = dynamic_cast<Game&>(Engine::Get());
	int screenWidth = game.GetFrameWidth();
	int screenHeight = game.GetFrameHeight();
	// 카메라 위치 이동
	if (cameraPosition.x < 427 - screenWidth)
	{
		// 위치 이동(float)
		cameraAccumX += cameraSpeed * deltaTime;
		// int형으로 소수점 버림
		int move = (int)cameraAccumX;
		// int 로 바꿧는데 0이상이면 이동시키고 Camerax 소수점 유지시킴
		if (move > 0) 
		{
			CameraX += move; // 카메라 위치 ++
			cameraAccumX -= move; // 소수점 남겨놓기
		}
		// 위치 업데이트
		cameraPosition.x = CameraX;
	}
}

void GameLevel::Draw()
{
	Level::Draw();
	// 초당프레임수 확인
	Renderer::Get().Submit(L"FPS: " + std::to_wstring(FPS), Vector2(0, 0), Color::RED);
}

void GameLevel::LoadMap(const std::string& filename)
{
	std::string path = "../Assets/" + filename;
	FILE* file = nullptr;
	// 파일 열기
	fopen_s(&file, path.c_str(), "rt");
	if (!file) return; // 없으면 리턴

	// 맨첨부터 읽어서 파일 길이 반환해오기
	fseek(file, 0, SEEK_END);
	long fileLength = ftell(file);
	rewind(file); // 첨으로 ㄱㄱ

	// 파일 길이만큼 데이터 읽어오기
	char* buffer = new char[fileLength + 1];
	// buffer배열에 들어가고, 읽은 사이즈를 반환한다.
	size_t readSize = fread(buffer, sizeof(char), fileLength, file);
	buffer[readSize] = '\0'; //맨 끝에 종단 문자 넣기

	// 읽은거 없으면 정지
	assert(readSize > 0 && "No data in the stage file.");

	int index = 0;
	Vector2	position = Vector2(0, 2);

	// 사이즈 다 읽을때까지..
	while (index < (int)readSize)
	{
		char mapChar = buffer[index++];

		if (mapChar == '\n')
		{
			position.y++;
			position.x = 0;
			continue;
		}
		switch (mapChar)
		{
		case '#':
			SpawnActor<Wall>(position);
			break;
		case 'P':
			SpawnActor<Mole>(position);
			break;
		case 'D':
			SpawnActor<Dirt>(position);
			break;
		}
		position.x++;
	}

	delete[] buffer;
	buffer = nullptr;
	fclose(file);
	file = nullptr;

}

void GameLevel::FrameRate(float deltaTime)
{
	Renderer::Get().Submit(L"FrameRate: " + std::to_wstring(1.0f / deltaTime), Vector2(0, 0), Color::RED);

}

bool GameLevel::CanMove(const Craft::Vector2& playerPosition, const Craft::Vector2& nextPosition)
{
	Vector2 dir = nextPosition - playerPosition;
	Vector2 newPosition = playerPosition + dir;
	for (std::shared_ptr<Actor> actor : actorList)
	{
		if (actor->GetPosition() == newPosition)
		{
			if (actor->IsTypeOf<Wall>())
				return false;
		}
	}
	return true;
}

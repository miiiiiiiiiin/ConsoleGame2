#include <cassert>
#include <algorithm>
#include "GameLevel.h"
#include <Game/Game.h>
#include <Actor/Mole.h>
#include <Actor/Wall.h>

using namespace Craft;
void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	LoadMap("Map.txt");
}

void GameLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	// 카메라 이동
	UpdateCamera(deltaTime);
}

void GameLevel::UpdateCamera(float deltaTime)
{
	cameraElapsedTime += deltaTime;
	// 프레임 크기 가져오기
	Game& game = dynamic_cast<Game&>(Engine::Get());
	int screenWidth = game.GetFrameWidth();
	int screenHeight = game.GetFrameHeight();

	// 카메라 위치 이동
	if (cameraPosition.x < 427 - screenWidth)
	{
		cameraPosition.x += static_cast<int>(cameraSpeed * cameraElapsedTime);
	}
	// 총 길이 넘지 않도록 제한
	//cameraPosition.x = std::min(cameraPosition.x, 427 - screenWidth);

}
void GameLevel::Draw()
{
	Level::Draw();
	// GameLevel::Draw()에 임시로 추가
	//Renderer::Get().Submit(L"dt: " + std::to_wstring(deltaTime), Vector2(0, 20), Color::RED);
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
	Vector2	position = Vector2(0, 0);

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
		}
		position.x++;
	}

	delete[] buffer;
	buffer = nullptr;
	fclose(file);
	file = nullptr;

}

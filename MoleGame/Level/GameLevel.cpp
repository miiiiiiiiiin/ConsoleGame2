#include <cassert>
#include <algorithm>
#include "GameLevel.h"
#include <Game/Game.h>
#include <Actor/Mole.h>
#include <Actor/Wall.h>
#include <Actor/Dirt.h>
#include <Actor/BombPlacement.h>

using namespace Craft;
void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	LoadMap("Map.txt");
	//SpawnActor<BombPlacement>(Input::Get().GetMousePosition());

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
	UpdateVisibleActors();
	// 좌표 0부터 카메라포지션 이전값까지 액터들 다 false로 변경.
}

void GameLevel::Draw()
{
	Level::Draw();
	// 초당프레임수 확인
	Renderer::Get().Submit(L"FPS: " + std::to_wstring(FPS), Vector2(0, 0), Color::RED);
	UpdateVisibleActors();
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
	//cameraPosition.x = position.x;
	//cameraPosition.y = position.y;
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
		
		//ActorFactory::Create(mapChar, *this, position);

		switch (mapChar)
		{
			case '#':
			{
				//SpawnActor<Wall>(position) = false;
				//blockGrid[EncodePos(position.x, position.y)] = std::make_shared<Wall>(position);
				blockGrid[EncodePos(position.x, position.y)] = SpawnActor<Wall>(position);
				auto it = blockGrid.find(EncodePos(position.x, position.y));
				if (it != blockGrid.end())
				{
					it->second->SetActive(false);
				}
				break;

			}
			case 'P':
				SpawnActor<Mole>(position);
				break;
			case 'B':
				SpawnActor<BombPlacement>(position);
				break;
			case 'D':
			{
				blockGrid[EncodePos(position.x, position.y)] = SpawnActor<Dirt>(position);
				// 공간해싱 리스트에 추가
				auto it = blockGrid.find(EncodePos(position.x, position.y));
				if (it != blockGrid.end())
				{
					it->second->SetActive(false);
				}
				break;

			}
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

void GameLevel::UpdateVisibleActors()
{
	//actorList.clear();
	Game& game = dynamic_cast<Game&>(Engine::Get());
	int screenWidth = game.GetFrameWidth() + cameraPosition.x;
	
	int screenHeight = game.GetFrameHeight() + cameraPosition.y;
	for (int i = 0; i < cameraPosition.x; i++)
	{
		for (int j = 0; j < screenHeight; j++)
		{
			auto it = blockGrid.find(EncodePos(i, j));
			if (it != blockGrid.end())
			{
				//actorList.pop_back(it->second);
				it->second->SetActive(false);
			}
		}
	}
	for (int i = cameraPosition.x; i < screenWidth; i++)
	{
		for (int j = cameraPosition.y; j < screenHeight; j++)
		{
			auto it = blockGrid.find(EncodePos(i, j));
			if (it != blockGrid.end())
			{
				//actorList.pop_back(it->second);
				it->second->SetActive(true);
			}
		}
	}

	
}

bool GameLevel::CanMove(const Craft::Vector2& playerPosition, const Craft::Vector2& nextPosition)
{
	Vector2 dir = nextPosition - playerPosition;
	Vector2 newPosition = playerPosition + dir;
	for (std::shared_ptr<Actor> actor : actorList)
	{
		if (actor->GetPosition() == newPosition)
		{
			//if (actor->IsTypeOf<Dirt>() || actor->IsTypeOf<Wall>())
				return false;
		}
	}
	return true;
}

bool GameLevel::IsBombBlock()
{
	auto it = blockGrid.find(EncodePos(cameraPosition.x +Input::Get().GetMousePosition().x, cameraPosition.y + Input::Get().GetMousePosition().y));
	if (it != blockGrid.end())
	{
		if(it->second->IsTypeOf<Dirt>()) 
			return true;
	}
	return false;

	//for (std::shared_ptr<Actor> actor : actorList)
	//{
	//	//if (actor->IsTypeOf<Wall>())
	//	//{
	//	//	if(Input::Get().GetMousePosition() == actor->GetPosition())
	//	//		return false;
	//	//}
	//	if (actor->IsTypeOf<Dirt>())
	//	{
	//		if (Input::Get().GetMousePosition() == actor->GetPosition())
	//			return true;
	//	}
	//}
	//return false;
}

void GameLevel::BombBlockByQuadTree(const Vector2 Bombposition, std::vector<std::shared_ptr<Actor>>& result)
{
	Game& game = dynamic_cast<Game&>(Engine::Get());
	int screenWidth = game.GetFrameWidth();
	int screenHeight = game.GetFrameHeight();

	// 카메라 좌표 (루트 노드)
	bound bounds{ cameraPosition, screenWidth, screenHeight };

	QuadTreeNode root(bounds);
	std::vector<std::shared_ptr<Actor>> nowActor;
	// 루트에 액터 추가.(200번 추가..??)

	for (int i = cameraPosition.x; i < screenWidth; i++)
	{
		for (int j = cameraPosition.y; j < screenHeight; j++)
		{
			auto it = blockGrid.find(EncodePos(i, j));
			if (it != blockGrid.end())
			{
				if (it->second->IsTypeOf<Dirt>())
					root.Insert(it->second);
			}
		}
	}
	QuadTreeNode* ContainNode = root.FindActor(Bombposition);
	if (!ContainNode)
	{
		OutputDebugStringA("ContainNode is NULL\n");
		return;
	}
	// 폭탄 위치 갖다주기
	result = ContainNode->Getactors();


}

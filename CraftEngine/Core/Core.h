#pragma once

//4251경고 끄기
#pragma warning(disable: 4251)

//빌드 모드에 따라서 변경될 값
#define DLLEXPORT __declspec(dllexport) // 디버그모드일때만 _declspec 드가고 릴리즈는 삭제
#define DLLIMPORT __declspec(dllimport)

//특정 값에 따라 분기하도록 처리
#if ENGINE_BUILD_DLL 
#define CRAFT_API DLLEXPORT
#else
#define CRAFT_API DLLIMPORT
#endif
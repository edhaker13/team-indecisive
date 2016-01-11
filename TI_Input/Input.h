#pragma once
#include <windows.h>
#ifdef TI_INPUT_EXPORTS
#	define INPUT_API __declspec(dllexport)
#else
#	define INPUT_API __declspec(dllimport)
#endif

#define IS_KEY_DOWN(virt_key_code) ((GetASyncKeyState(virt_key_code) & 0x8000) ? 1 : 0)
#define IS_KEY_UP(virt_key_code) ((GetASyncKeyState(virt_key_code) & 0x8000) ? 0 : 1)

namespace Indecisive
{
	class Input
	{
	public:
		INPUT_API Input();
		~Input();

		INPUT_API bool IsKeyDown(char c);
		INPUT_API bool IsKeyDownEx(char c);
		INPUT_API void Update();

	private:
		int* oldKeyState;
		int* keyState;
	};
}
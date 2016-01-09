#pragma once
#include <windows.h>

#define IS_KEY_DOWN(virt_key_code) ((GetASyncKeyState(virt_key_code) & 0x8000) ? 1 : 0)
#define IS_KEY_UP(virt_key_code) ((GetASyncKeyState(virt_key_code) & 0x8000) ? 0 : 1)

namespace Indecisive
{
	class Input
	{
	public:
		Input();
		~Input();

		bool IsKeyDown(char c);
		bool IsKeyDownEx(char c);
		void Update();

	private:
		int* oldKeyState;
		int* keyState;
	};
}
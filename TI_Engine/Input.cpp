#include "Input.h"

namespace Indecisive
{
	Input::Input()
	{
		oldKeyState = new int[256];
		keyState = new int[256];

		for (int i = 0; i < 256; i++)
		{
			oldKeyState[i] = 1;
			keyState[i] = 1;
		}
	}

	Input::~Input()
	{
		return;
	}

	void Input::Update()
	{
		int* swap;
		swap = oldKeyState;
		oldKeyState = keyState;
		keyState = swap;

		for (int i = 0; i < 256; i++)
		{
			keyState[i] = GetAsyncKeyState(i) & 0x8000 ? 2 : 1;
		}
	}

	bool Input::IsKeyDown(char c)
	{
		if (keyState[c] == 2)
		{
			return true;
		}
		else return false;
	}

	bool Input::IsKeyDownEx(char c)
	{
		if (keyState[c] == 2 && oldKeyState[c] != 2)
		{
			return true;
		}
		else return false;
	}
}
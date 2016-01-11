#pragma once
#include <map>
#include <windows.h>
#include "..\TI_Engine\IInputManager.h"

#ifdef TI_INPUT_EXPORTS
#	define INPUT_API __declspec(dllexport)
#else
#	define INPUT_API __declspec(dllimport)
#endif

namespace Indecisive
{
	class InputManager: public IInputManager
	{
	public:
		INPUT_API InputManager() {};
		INPUT_API bool IsKeyDown(KeyCode vKey) override;
		INPUT_API void KeyPress(KeyCode vKey) override;
		INPUT_API void RegisterAction(KeyCode vKey, InputCallback func) override;
	private:
		std::map<KeyCode,InputCallback> mKeyActions;
	};
};
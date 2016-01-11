#include "InputManager.h"

namespace Indecisive
{
	bool InputManager::IsKeyDown(KeyCode vKey) { return (GetAsyncKeyState(vKey) & 0x0800) == 1; };
	void InputManager::RegisterAction(KeyCode vKey, InputCallback func) { mKeyActions.emplace(vKey, func); };
	void InputManager::KeyPress(KeyCode vKey)
	{
		if (mKeyActions.find(vKey) != mKeyActions.end()) mKeyActions[vKey]();
	}
}
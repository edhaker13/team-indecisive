#pragma once
#include <windows.h>
#include "IGraphics.h"

namespace Indecisive
{
	class GameManager
	{
	private:
		IGraphics* _pGraphics = nullptr;

	public:
		GameManager(HINSTANCE hInstance, int nCmdShow);
		void Draw();
		void Update();
		void SetTimer();
	};
}

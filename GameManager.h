#pragma once
#include "IGraphics.h"
#include <windows.h>

namespace Indecisive
{
	class GameManager
	{
	private:
		IGraphics* _pGraphics = nullptr;

	public:
		int Initialise(HINSTANCE hInstance, int nCmdShow);
		void Draw();
		void Update();
		void SetTimer();
	};
}

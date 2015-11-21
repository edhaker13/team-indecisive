#pragma once
#include "..\TI_Engine\IGraphics.h"
#include <windows.h>

namespace Indecisive
{
	class GameManager
	{
	private:
		IGraphics* _pGraphics = nullptr;

	public:
		HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);
		void Draw();
		void Update();
		void SetTimer();
	};
}
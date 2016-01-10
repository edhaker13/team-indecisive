#pragma once
#include <windows.h>
#include "..\TI_Engine\IGraphics.h"
#undef main

namespace Indecisive
{
	class GameManager
	{
	private:
		// Graphics interface. Assigned after constructor.
		IGraphics* _pGraphics = nullptr;

	public:
		GameManager(HINSTANCE hInstance, int nCmdShow);
		void Draw() const;
		void Update();
		void SetTimer();
	};
}

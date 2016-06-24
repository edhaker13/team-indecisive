#pragma once
#include <windows.h>
#include "..\TI_Engine\IGraphics.h"

namespace Indecisive
{
	class GameManager
	{
	private:
		// Graphics interface pointer. Assigned after constructor.
		IGraphics* _pGraphics = nullptr;

	public:
		GameManager(HINSTANCE hInstance, int nCmdShow);
		void Draw() const; // Wrap call to graphics interface
		void Update(); // Wrap call to graphics interface
		void SetTimer(); // Allocate timer class
	};
}

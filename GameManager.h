#pragma once
//#include "resource.h"
#include "IGraphics.h"
#include <windows.h>

namespace Indecisive
{
	class GameManager
	{
	public:
		void Initialise(IGraphics*);

		void Draw(IGraphics*);
		void Update(IGraphics*);

		void SetTimer(IGraphics*);

		//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
	};
}

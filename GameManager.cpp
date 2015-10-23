#include "GameManager.h"
#include "GraphicsDirectX.h"
#include "ServiceLocator.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Indecisive::GraphicsDirectX* theApp = new Indecisive::GraphicsDirectX();

	Indecisive::GameManager _gameMngr = Indecisive::GameManager();

	_gameMngr.Initialise(theApp);

	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}

	// Main message loop
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			_gameMngr.Draw(theApp);
			_gameMngr.Update(theApp);
		}
	}

	return (int)msg.wParam;
}

namespace Indecisive
{
	void GameManager::Initialise(IGraphics* _pGraphics)
	{
		ServiceLocator::Instance()->Add("graphics", _pGraphics);
	};

	void GameManager::Draw(IGraphics* _pGraphics)
	{
		_pGraphics->Draw();
	};

	void GameManager::Update(IGraphics* _pGraphics)
	{
		_pGraphics->Update();
	};

	void GameManager::SetTimer(IGraphics* _pGraphics)
	{

	};
}
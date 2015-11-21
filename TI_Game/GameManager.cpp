#include "GameManager.h"
#include "..\TI_Engine\GraphicsDirectX.h"
#include "..\TI_Engine\ServiceLocator.h"
#include "..\TI_Engine\Window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto _gameMngr = Indecisive::GameManager();
	auto hr = _gameMngr.Initialise(hInstance, nCmdShow);

	if (FAILED(hr))
	{
		return hr;
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
			_gameMngr.Draw();
			_gameMngr.Update();
		}
	}

	return (int)msg.wParam;
}

namespace Indecisive
{
	HRESULT GameManager::Initialise(HINSTANCE hInstance, int nCmdShow)
	{
		_pGraphics = new GraphicsDirectX();
		auto pWindow = new Window();
		ServiceLocator::Instance()->Add("graphics", _pGraphics);
		//ServiceLocator::Instance()->Add("window", pWindow);
		
		if (FAILED(pWindow->Initialise(hInstance, nCmdShow)))
		{
			return E_FAIL;
		}
		if (!_pGraphics->Initialise(pWindow))
		{
			return E_FAIL;
		}
		return S_OK;
	};

	void GameManager::Draw()
	{
		_pGraphics->Draw();
	};

	void GameManager::Update()
	{
		_pGraphics->Update();
	};

	void GameManager::SetTimer()
	{
	};
}
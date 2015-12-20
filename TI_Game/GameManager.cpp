#include "GameManager.h"
#include "LevelLoader.h"
#include "Window.h"
/*
#include "ComponentFactory.h"
#include "GraphicsDirectX.h"
#include "SceneGraph.h"
#include "ServiceLocator.h"
*/
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto _gameMngr = Indecisive::GameManager(hInstance, nCmdShow);

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
			// Cam Movement
		}
	}

	return (int)msg.wParam;
}

namespace Indecisive
{
	GameManager::GameManager(HINSTANCE hInstance, int nCmdShow)
	{
		LevelLoader lvlLoader;
		static const std::string level = "dummy.lvl";

		if (!lvlLoader.CanRead(level))
		{
			throw std::invalid_argument("Can't read level file:" + level);
		}

		lvlLoader.GetWindow(level)->Initialise(hInstance, nCmdShow);
		lvlLoader.Read(level);

		_pGraphics = lvlLoader.GetGraphics();
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
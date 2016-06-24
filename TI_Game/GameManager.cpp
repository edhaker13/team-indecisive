#include "GameManager.h"
#include "..\TI_Engine\Logger.h"
#include "..\TI_Engine\Window.h"
#include "..\TI_Loader\LevelLoader.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto _gameMngr = Indecisive::GameManager(hInstance, nCmdShow);
	// Main message loop
	MSG msg = { 0 };
	// Ignore most messages at this level
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
	// TODO: Not really a manager so why the name
	GameManager::GameManager(HINSTANCE hInstance, int nCmdShow)
	{
		// Allocate level object loader
		auto lvlLoader = LevelLoader();
		// TODO: Make this constant a parameter?
		static const std::string level = "dummy.lvl";

		if (!lvlLoader.CanRead(level))
		{
			TI_LOG_F("Wrong file type for level file: " + level);
		}

		try
		{
			lvlLoader.ReadWindow(level);
			lvlLoader.GetWindow()->Initialise(hInstance, nCmdShow);
			lvlLoader.Read(level);
		}
		catch (const std::exception& ex)
		{
			TI_LOG_EXCEPTION("failed to read level.", ex);
		}
		try
		{
			_pGraphics = lvlLoader.GetGraphics();
		}
		catch (const std::exception& ex)
		{
			TI_LOG_EXCEPTION("failed to allocate graphics interface.", ex);
		}
		TI_LOG_V("Loaded Level: " + level);
	};
	
	void GameManager::Draw() const
	{
		_pGraphics->Draw();
	};

	void GameManager::Update()
	{
		_pGraphics->Update();
	};

	void GameManager::SetTimer()
	{
		//TODO: TIMER CLASS
	};
}
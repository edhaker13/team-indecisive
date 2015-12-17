#include "ComponentFactory.h"
#include "GameManager.h"
#include "GraphicsDirectX.h"
#include "SceneGraph.h"
#include "Window.h"

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
			// Cam Movement
		}
	}

	return (int)msg.wParam;
}

namespace Indecisive
{
	HRESULT GameManager::Initialise(HINSTANCE hInstance, int nCmdShow)
	{
		// TODO: Load details from file
		_pGraphics = new GraphicsDirectX();
		auto pWindow = new Window(1280, 720, L"Team Indecisive");
		ServiceLocatorInstance()->Add("graphics", _pGraphics);
		// Initialize the camera node
		auto pos = new PositionNode("move", Vector3(0.0f, 0.0f, 50.0f));
		auto cam = new CameraNode("camera", Vector3(0.0f, 100.0f, -150.0f), Vector3::Zero, Vector3::Up, 10.0f, 1000.0f);
		ServiceLocatorInstance()->Add("camera", cam);
		
		if (FAILED(pWindow->Initialise(hInstance, nCmdShow)))
		{
			return E_FAIL;
		}
		if (!_pGraphics->Initialise(pWindow))
		{
			return E_FAIL;
		}
		// Initialise game objects (OBJLoader makes uses of graphics so it needs to be done after it)
		auto obj = ComponentFactory::MakeTestObjectFromObj("fullcar.obj");

		// Construct tree. Cam -> Pos -> Obj
		pos->Append(new ObjectNode("car", *obj));
		cam->Append(pos);

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
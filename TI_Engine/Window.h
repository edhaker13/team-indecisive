#pragma once
#include <windows.h>
#include "resource.h"

namespace Indecisive
{
	inline LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	};

	class Window
	{
	private:
		HWND _hWnd = nullptr;
		UINT _windowWidth = 720;
		UINT _windowHeight = 540;
		LPWSTR _windowName = L"Team Indecisve";
	public:
		Window(UINT width, UINT height, LPWSTR name) : _windowWidth(width), _windowHeight(height), _windowName(name) {};
		HRESULT Initialise(HINSTANCE hInstance, int nCmdShow)
		{
			// Register class
			WNDCLASSEX wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDC_TUTORIAL1);
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = _windowName;
			wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDC_TUTORIAL1);
			if (!RegisterClassEx(&wcex))
				return E_FAIL;

			// Create window
			//_hInst = hInstance;
			RECT rc = { 0, 0, _windowWidth, _windowHeight };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
			_hWnd = CreateWindow(_windowName, _windowName, WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
				nullptr);
			if (!_hWnd)
				return E_FAIL;

			ShowWindow(_hWnd, nCmdShow);

			return S_OK;
		};

		const HWND GetHWND() { return _hWnd; }
		const UINT GetWidth() { return _windowWidth; }
		const UINT GetHeight() { return _windowHeight; }
	};
}
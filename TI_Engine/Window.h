#pragma once
#include <windows.h>
#include "resource.h"
#include "Logger.h"

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
		const std::wstring _windowName = L"Team Indecisve";
	public:
		Window(UINT width, UINT height, const std::wstring& name) :
			_windowWidth(width), _windowHeight(height), _windowName(name) {};
		void Initialise(HINSTANCE hInstance, int nCmdShow)
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
			wcex.lpszClassName = _windowName.c_str();
			wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDC_TUTORIAL1);
			if (!RegisterClassEx(&wcex))
			{
				auto winApiError = GetLastError();
				TI_LOG_E("Failed to register window class. WINAPI ERROR: " << winApiError);
				throw winApiError;
			}

			// Create window
			RECT rc = { 0, 0, _windowWidth, _windowHeight };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
			_hWnd = CreateWindow(_windowName.c_str(), _windowName.c_str(), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
			if (!_hWnd)
			{
				auto winApiError = GetLastError();
				TI_LOG_E("Failed to create window. WINAPI ERROR: " << winApiError);
				throw winApiError;
			}

			if (ShowWindow(_hWnd, nCmdShow) < 0)
			{
				auto winApiError = GetLastError();
				TI_LOG_E("Failed to show window. WINAPI ERROR: " << winApiError);
				throw winApiError;
			}
		};

		HWND GetHWND() const { return _hWnd; }
		UINT GetWidth() const { return _windowWidth; }
		UINT GetHeight() const { return _windowHeight; }
	};
}
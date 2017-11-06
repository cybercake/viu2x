/* Copyright (C) Hao Qin. All rights reserved. */

#include "App.h"

namespace v2x {

#ifdef V2X_WINDOWS

	const wchar_t * VIU2X_WINDOW_CLASS_NAME = L"Viu2x_Common_Window";

	LRESULT CALLBACK Viu2xWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		int wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		//case WM_COMMAND:
		//	wmId = LOWORD(wParam);
		//	wmEvent = HIWORD(wParam);
		//	// Parse the menu selections:
		//	switch (wmId)
		//	{
		//	case IDM_ABOUT:
		//		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		//		break;
		//	case IDM_EXIT:
		//		DestroyWindow(hWnd);
		//		break;
		//	default:
		//		return DefWindowProc(hWnd, message, wParam, lParam);
		//	}
		//	break;
		//case WM_PAINT:
		//	hdc = BeginPaint(hWnd, &ps);
		//	// TODO: Add any drawing code here...
		//	EndPaint(hWnd, &ps);
		//	break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	class OsWindow;

	std::map<HWND, std::shared_ptr<OsWindow>> TopLevelWindows;

	class OsWindow : public Window {
	public:
		DEFINE_POINTERS(OsWindow);

		OsWindow(HINSTANCE hInstance) {

			m_hwnd = CreateWindowW(VIU2X_WINDOW_CLASS_NAME, L"viu2x", WS_OVERLAPPEDWINDOW, 
				CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
			if (m_hwnd == NULL)
				OsException::throwLatest(L"OsWindow::OsWindow()");
		}

		virtual ~OsWindow() {

			DestroyWindow(m_hwnd);
		}

		void show() {

			ShowWindow(m_hwnd, SW_SHOW);
		}

		HWND getHandle() const {
			return m_hwnd;
		}

	private:

		HWND m_hwnd;
	};
	
#else
#error Not implemented!
#endif


	App::App() {
	
#ifdef V2X_WINDOWS
		
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(wcex);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Viu2xWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hIcon = NULL;
		wcex.hCursor = NULL;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = VIU2X_WINDOW_CLASS_NAME;
		wcex.hIconSm = NULL;

		if (!RegisterClassExW(&wcex))
			OsException::throwLatest(L"OsWindow::OsWindow()");
#else
#error Not implemented!
#endif
	}

	App::~App() {

#ifdef V2X_WINDOWS
		HINSTANCE hInstance = GetModuleHandle(NULL);
		UnregisterClass(VIU2X_WINDOW_CLASS_NAME, hInstance);
#else
#error Not implemented!
#endif
	}

	Window::Shared App::createWindow() {

#ifdef V2X_WINDOWS
		HINSTANCE hInstance = GetModuleHandle(NULL);
		OsWindow::Shared window (new OsWindow(hInstance));
		TopLevelWindows[window->getHandle()] = window;
		return window;
#else
#error Not implemented!
#endif
	}

	void App::run() {
		
#ifdef V2X_WINDOWS
		MSG msg;

		// Main message loop:
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#else
#error Not implemented!
#endif
	}
}
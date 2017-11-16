/* Copyright (C) Hao Qin. All rights reserved. */

#include "../../common.h"

#ifdef V2X_WINDOWS

#include "Controls.h"
#include "WindowHostWinGdi.h"
#include "../Displays.h"

namespace v2x {

	std::recursive_mutex WindowHostWinGdi::g_ownerThreadIdMutex;
	bool WindowHostWinGdi::g_ownerThreadIdIsSet = false;
	std::thread::id WindowHostWinGdi::g_ownerThreadId;
	String WindowHostWinGdi::g_windowClassName = L"";
	HINSTANCE WindowHostWinGdi::g_hInstance = 0;
	std::map<HWND, std::shared_ptr<WindowHostWinGdi>> WindowHostWinGdi::g_topLevelWindows;

	void WindowHostWinGdi::assertGuiThread(const String & caller) {
		std::lock_guard<std::recursive_mutex> lock(g_ownerThreadIdMutex);
		if (!g_ownerThreadIdIsSet)
			throw Exception(L"%s: The GUI system has not been initialized!", caller.c_str());
		if (g_ownerThreadId != std::this_thread::get_id())
			throw Exception(L"%s: The App instance can be freed ONLY in the owner thread!", caller.c_str());
	}

	bool WindowHostWinGdi::sendMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

		//#define APPWINDOWS_SAFE_SENDMESSAGE
#ifdef APPWINDOWS_SAFE_SENDMESSAGE
		// This is a safe implementation
		auto w = g_topLevelWindows.find(hWnd);
		if (w != g_topLevelWindows.end()) {
			return w->second->processWindowsMessage(message, wParam, lParam);
		}
		return false;
#else
		return g_topLevelWindows[hWnd]->processWindowsMessage(message, wParam, lParam);
#endif
	}

	LRESULT CALLBACK WindowHostWinGdi::Viu2xWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

		bool handled = sendMessage(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_DESTROY:
			g_topLevelWindows.erase(hWnd);
			if (g_topLevelWindows.empty())
				PostQuitMessage(0);
			break;

		default:
			if (!handled)
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void WindowHostWinGdi::doMessageLoop() {

		assertGuiThread(L"WindowHostWinGdi::doMessageLoop()");

		MSG msg;

		// Main message loop
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	WindowHostWinGdi::WindowHostWinGdi(HWND & newHandle) {

		assertGuiThread(L"WindowHostWinGdi::unregisterWindowClass()");

		m_hwnd = CreateWindowW(g_windowClassName.c_str(), L"", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, g_hInstance, NULL);
		if (m_hwnd == NULL)
			OsException::throwLatest(L"WindowHostWinGdi::WindowHostWinGdi()");

		newHandle = m_hwnd;
	}

	WindowHostWinGdi::~WindowHostWinGdi() {
	}

	void WindowHostWinGdi::registerWindowClass() {

		// Check if another App has been created
		std::lock_guard<std::recursive_mutex> lock(g_ownerThreadIdMutex);
		if (g_ownerThreadIdIsSet)
			throw Exception(L"WindowHostWinGdi::unregisterWindowClass(): An App instance has been created!");

		g_hInstance = GetModuleHandle(NULL);
		g_windowClassName = StrUtils::format(L"Viu2x_Common_Window_%d", g_hInstance);

		// Register class
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Viu2xWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = g_hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = g_windowClassName.c_str();
		wcex.hIconSm = NULL;
		if (!RegisterClassExW(&wcex))
			OsException::throwLatest(L"WindowHostWinGdi::WindowHostWinGdi()");

		// Update global information
		g_ownerThreadId = std::this_thread::get_id();
		g_ownerThreadIdIsSet = true;
	}

	void WindowHostWinGdi::unregisterWindowClass() {

		assertGuiThread(L"WindowHostWinGdi::unregisterWindowClass()");

		UnregisterClass(g_windowClassName.c_str(), g_hInstance);
	}

	WindowHostWinGdi::Shared WindowHostWinGdi::createNew() {

		assertGuiThread(L"WindowHostWinGdi::unregisterWindowClass()");

		HWND newHandle;
		WindowHostWinGdi::Shared result(new WindowHostWinGdi(newHandle));

		// Unfortunately we cannot do this in OsWindow::OsWindow()
		g_topLevelWindows[newHandle] = result;

		return result;
	}

	void WindowHostWinGdi::show() {

		if (m_hwnd == NULL)
			throw Exception(L"WindowHostWinGdi::show(): The native window handle is not initialized!");

		ShowWindow(m_hwnd, SW_SHOW);
	}

	void WindowHostWinGdi::close() {

		if (m_hwnd == NULL)
			throw Exception(L"WindowHostWinGdi::show(): The native window handle is not initialized!");

		CloseWindow(m_hwnd);
		m_hwnd = NULL;
	}

	void WindowHostWinGdi::setPosition(const Rect64F & position) {

		if (m_hwnd == NULL)
			throw Exception(L"WindowHostWinGdi::show(): The native window handle is not initialized!");

		Vector2D64F pos(std::floor(position.getLeft()), std::floor(position.getTop()));
		Rect32I newRect((int)pos.x, (int)pos.y,
			(int)std::ceil(position.getWidth() - pos.x),
			(int)std::ceil(position.getHeight() - pos.y));
		SetWindowPos(m_hwnd, HWND_TOP,
			newRect.getLeft(), newRect.getTop(), newRect.getWidth(), newRect.getHeight(),
			SWP_NOACTIVATE);
	}

	bool WindowHostWinGdi::processWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam) {

		switch (message) {

		case WM_SHOWWINDOW:
		{
			RECT rect;
			memset(&rect, 0, sizeof(rect));
			GetWindowRect(m_hwnd, &rect);
			EventDataWindowSize::Shared data(new EventDataWindowSize(WindowState::Normal,
				Vector2D64F(rect.left, rect.top),
				Size2D64F(rect.right - rect.left, rect.bottom - rect.top)));

			OnShow.notifyEvent(Event::Shared(new Event(shared_from_this(), data)));
			return false;
		}

		case WM_CLOSE:
			OnClose.notifyEvent(Event::Shared(new Event(shared_from_this(), Object::Shared())));
			return false;

			// + Mouse
			// + Keyboard
			// + Paint
			// + Window Resize

			// + State changes: Maximize/Minimize/Close/Activate/Deactivate
		case WM_MOVE:
		case WM_SIZE:
		{
			WindowState state;
			switch (wParam) {
			case SIZE_MAXIMIZED: state = WindowState::Maximized;
			case SIZE_MINIMIZED: state = WindowState::Minimized;
			default: state = WindowState::Normal;
			}

			RECT rect;
			memset(&rect, 0, sizeof(rect));
			GetWindowRect(m_hwnd, &rect);
			EventDataWindowSize::Shared data(new EventDataWindowSize(state,
				Vector2D64F(rect.left, rect.top),
				Size2D64F(rect.right - rect.left, rect.bottom - rect.top)));

			OnResize.notifyEvent(Event::Shared(new Event(shared_from_this(), data)));
			return false;
		}

		default:
			return false;
		}
	}
}

#endif
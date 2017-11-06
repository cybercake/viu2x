/* Copyright (C) Hao Qin. All rights reserved. */

#include "../../common.h"

#ifdef V2X_WINDOWS

#include "App.h"
#include "AppWindows.h"

#include <thread>
#include <mutex>
#include <map>

namespace v2x {

	///////////////////
	// WindowHostWin //
	///////////////////

	class WindowHostWin;

	namespace {

		/// A mutex to protect the owner thread ID initialzation.
		std::recursive_mutex g_ownerThreadIdMutex;

		/// A flag indicating if the owner thread ID has been set.
		bool g_ownerThreadIdIsSet = false;

		/// The owner thread ID of the App instance. It'll be set only once at 
		/// startup.
		std::thread::id g_ownerThreadId;

		/// The window class name for the current process. Different viu2x 
		/// process instance will register different window class name.
		String g_windowClassName = L"";

		/// The handle to the current process.
		HINSTANCE g_hInstance = 0;

		/// A map from HWND for all valid top level window objects.
		/// It is used to dispatch the message from the WndProc to the proper 
		/// WindowHost instance.
		std::map<HWND, std::shared_ptr<WindowHostWin>> g_topLevelWindows;

		/// This function is used to assert if the calling thread is the one 
		/// who created the App instance.
		void assertThread(const String & caller) {
			std::lock_guard<std::recursive_mutex> lock(g_ownerThreadIdMutex);
			if (!g_ownerThreadIdIsSet)
				throw Exception(L"%s: The GUI system has not been initialized!", caller.c_str());
			if (g_ownerThreadId != std::this_thread::get_id())
				throw Exception(L"%s: The App instance can be freed ONLY in the owner thread!", caller.c_str());
		}

		// The common WndProc for all WindowHost objects
		LRESULT CALLBACK Viu2xWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
			switch (message)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			return 0;
		}
	}

	WindowHostWin::WindowHostWin() {

		m_hwnd = CreateWindowW(g_windowClassName.c_str(), L"", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, g_hInstance, NULL);
		if (m_hwnd == NULL)
			OsException::throwLatest(L"OsWindow::OsWindow()");
	}

	WindowHostWin::~WindowHostWin() {

		DestroyWindow(m_hwnd);
	}

	void WindowHostWin::show() {

		ShowWindow(m_hwnd, SW_SHOW);
	}

	void WindowHostWin::close() {

		CloseWindow(m_hwnd);
		g_topLevelWindows.erase(m_hwnd);
	}

	HWND WindowHostWin::getHandle() const {
		return m_hwnd;
	}

	/////////
	// App //
	/////////

	App::App() {

		// Check if another App has been created
		std::lock_guard<std::recursive_mutex> lock(g_ownerThreadIdMutex);
		if (g_ownerThreadIdIsSet)
			throw Exception(L"App::App(): An App instance has been created!");

		g_hInstance = GetModuleHandle(NULL);
		g_windowClassName = StrUtils::format(L"Viu2x_Common_Window_%d", g_hInstance);

		// Register class
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
		wcex.lpszClassName = g_windowClassName.c_str();
		wcex.hIconSm = NULL;
		if (!RegisterClassExW(&wcex))
			OsException::throwLatest(L"OsWindow::OsWindow()");

		// Update global information
		g_ownerThreadId = std::this_thread::get_id();
		g_ownerThreadIdIsSet = true;
	}

	App::~App() {

		assertThread(L"App::~App()");

		UnregisterClass(g_windowClassName.c_str(), g_hInstance);
	}

	WindowHost::Shared App::createWindowHost() {

		assertThread(L"App::createWindow()");

		WindowHostWin::Shared window(new WindowHostWin());

		// Unfortunately we cannot do this in OsWindow::OsWindow()
		g_topLevelWindows[window->getHandle()] = window;

		return window;
	}

	void App::waitUntilTermination() {

		assertThread(L"App::run()");

		MSG msg;

		// Main message loop
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

#endif
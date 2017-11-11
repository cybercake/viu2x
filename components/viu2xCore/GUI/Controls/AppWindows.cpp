/* Copyright (C) Hao Qin. All rights reserved. */

#include "../../common.h"

#ifdef V2X_WINDOWS

#include "App.h"
#include "AppWindows.h"
#include "../Displays.h"

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

		/// This function forwards the message to the target window host
		bool sendMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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

		/// The common WndProc for all WindowHost objects
		LRESULT CALLBACK Viu2xWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
	}

	///////////////////
	// WindowHostWin //
	///////////////////

	WindowHostWin::WindowHostWin() {

		m_hwnd = CreateWindowW(g_windowClassName.c_str(), L"", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, g_hInstance, NULL);
		if (m_hwnd == NULL)
			OsException::throwLatest(L"OsWindow::OsWindow()");
	}

	WindowHostWin::~WindowHostWin() {
	}

	void WindowHostWin::show() {

		if (m_hwnd == NULL)
			throw Exception(L"WindowHostWin::show(): The native window handle is not initialized!");

		ShowWindow(m_hwnd, SW_SHOW);
	}

	void WindowHostWin::close() {

		if (m_hwnd == NULL)
			throw Exception(L"WindowHostWin::show(): The native window handle is not initialized!");

		CloseWindow(m_hwnd);
		m_hwnd = NULL;
	}

	void WindowHostWin::setPosition(const Rect64F & position) {
		
		if (m_hwnd == NULL)
			throw Exception(L"WindowHostWin::show(): The native window handle is not initialized!");

		Vector2D64F pos(std::floor(position.getLeft()), std::floor(position.getTop()));
		Rect32I newRect((int)pos.x, (int)pos.y, 
			(int)std::ceil(position.getWidth() - pos.x), 
			(int)std::ceil(position.getHeight() - pos.y));
		SetWindowPos(m_hwnd, HWND_TOP, 
			newRect.getLeft(), newRect.getTop(), newRect.getWidth(), newRect.getHeight(), 
			SWP_NOACTIVATE);
	}

	HWND WindowHostWin::getHandle() const {
		return m_hwnd;
	}

	bool WindowHostWin::processWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam) {

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

	Size2D64F App::getDefaultWindowSize() {

		Displays displays;
		Size2D32I workarea = displays.getPrimaryDisplay()->getWorkAreaInPx().size;
		Size2D64F result(workarea.width / 3, workarea.height / 3);
		return result;
	}

	WindowHost::Shared App::createWindowHost() {

		assertThread(L"App::createWindow()");

		WindowHostWin::Shared window(new WindowHostWin());

		// Unfortunately we cannot do this in OsWindow::OsWindow()
		g_topLevelWindows[window->getHandle()] = window;

		return window;
	}

	std::vector<String> App::getParameters() {

		std::vector<String> result;

		String cmd(GetCommandLine());
		cmd = cmd + L"\""; // A trick to process the last part of the command line

		bool isQuoted = false;
		int anchorPos = -1;

		for (size_t i = 0; i < cmd.length(); ++i) {

			switch (cmd[i]) {
			case ' ':
			case '\t':
				if (!isQuoted && anchorPos >= 0)
				{
					String param = cmd.substr(anchorPos, i - anchorPos);
					result.push_back(param);
					anchorPos = -1;
				}
				break;

			case '"':

				isQuoted = !isQuoted;
				if (anchorPos >= 0) {
					if (isQuoted) {
						String param = cmd.substr(anchorPos, i - anchorPos);
						result.push_back(param);
						anchorPos = -1;
					}
					else {
						String param = cmd.substr(anchorPos, i - anchorPos);
						result.push_back(param);
						anchorPos = -1;
					}
				}
				break;

			default:
				if (anchorPos < 0)
					anchorPos = i;
				continue;
			}
		}

		return result;
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
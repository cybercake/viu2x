/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"

#ifdef V2X_WINDOWS

#include "WindowHost.h"
#include <windows.h>

#include <thread>
#include <mutex>
#include <map>

namespace v2x {

	/// The WindowHost implementation for Windows
	///
	/// It should be instantiated only by App within the main thread.
	///
	class WindowHostWinGdi : public WindowHost {
	public:
		DEFINE_POINTERS(WindowHostWinGdi);

		/// This function creates a new WindowHostWinGdi instance. The new 
		/// window will be invisible.
		static WindowHostWinGdi::Shared createNew();

		// We always need a virtual destructor
		virtual ~WindowHostWinGdi();

		/// Windows-specific initialization. It can be called for multiple 
		/// times by the owner thread. In case it has been initialzed in that
		/// thread, it just do nothing. In case it is called in a non-owner 
		/// thread, an exception will be thrown.
		static void initialize();

		/// Windows-specific deinitialization. It must be called in the owner 
		/// thread. Once the system is deinitialized, it can be initialized 
		/// again by another thread.
		static void deinitialize();

		/// Process messages in the main loop and block until the last window 
		/// closes.
		static void doMessageLoop();

		// Show the native window and trigger the OnShow event
		void show() override;
		// Close the native window and trigger the OnClose event
		void close() override;
		// Change the native window size and trigger the OnResize event
		void setPosition(const Rect64F & position) override;
		/// This function returns the default window size of the v2x system.
		Size2D64F getDefaultWindowSize() override;

	protected:
		/// The constructor creates a new native window and export the handle 
		/// in newHandle. It should be call ONLY by the createNew() method.
		WindowHostWinGdi(HWND & newHandle);

	private:

		/// A mutex to protect the owner thread ID initialzation.
		static std::recursive_mutex g_ownerThreadIdMutex;

		/// A flag indicating if the owner thread ID has been set.
		static bool g_initialized;

		/// The owner thread ID of the App instance. It'll be set only once at 
		/// startup.
		static std::thread::id g_ownerThreadId;

		/// The window class name for the current process. Different viu2x 
		/// process instance will register different window class name.
		static String g_windowClassName;

		/// The handle to the current process.
		static HINSTANCE g_hInstance;

		/// A map from HWND for all valid top level window objects.
		/// It is used to dispatch the message from the WndProc to the proper 
		/// WindowHost instance.
		static std::map<HWND, std::shared_ptr<WindowHostWinGdi>> g_topLevelWindows;

		static void assertGuiThread(const String & caller);

		static bool sendMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static LRESULT CALLBACK Viu2xWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND m_hwnd;

		bool processWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam);
	};

}

#endif
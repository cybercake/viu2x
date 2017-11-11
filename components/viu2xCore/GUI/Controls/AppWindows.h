/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"
#include "App.h"

#ifdef V2X_WINDOWS

#include <windows.h>

namespace v2x {

	/// The WindowHost implementation for Windows
	///
	/// It should be instantiated only by App within the main thread.
	///
	class WindowHostWin : public WindowHost {
	public:
		DEFINE_POINTERS(WindowHostWin);

		WindowHostWin();

		virtual ~WindowHostWin();

		void show() override;
		void close() override;
		void setPosition(const Rect64F & position) override;

		HWND getHandle() const;

		bool processWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam);

	private:

		HWND m_hwnd;
	};

}

#endif
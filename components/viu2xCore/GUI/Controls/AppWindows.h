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

		void show();

		void close();

		HWND getHandle() const;

	private:

		HWND m_hwnd;
	};

}

#endif
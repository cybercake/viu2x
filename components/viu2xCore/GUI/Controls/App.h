/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"
#include "Controls.h"

#include <map>

#ifdef V2X_WINDOWS
#	include "windows.h"
#endif

namespace v2x {

	class App {
	public:
		DEFINE_POINTERS(App);

		App();

		virtual ~App();

		Window::Shared createWindow();

		void run();
	};
}
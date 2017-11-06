/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"
#include "Controls.h"

#include <thread>
#include <mutex>

namespace v2x {

	/// App is the host class for the GUI thread. It has a OS-independent 
	/// interface but highly OS-specific implementation.
	/// 
	/// App has to be instantiated in the MAIN thread. It will refuse the call 
	/// from any other threads.
	///
	///
	class App {
	public:
		/// The default constructor.
		/// It should be called in the WinMain function.
		App();

		/// We always need a virtual destructor.
		virtual ~App();

		/// This function creates a new Window instance. The new window will be 
		/// invisible. You need to call Window::show() to see it.
		static WindowHost::Shared createWindowHost();

		/// This function starts the main message loop and it returns after the 
		/// main window is closed.
		void waitUntilTermination();
	};
}
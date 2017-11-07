/* Copyright (C) Hao Qin. All rights reserved. */

#include "Controls.h"
#include "App.h"

#ifdef V2X_WINDOWS
#include "AppWindows.h"
#else
#error Not implemented!
#endif

namespace v2x {

	/////////////
	// Control //
	/////////////

	Control::Control() {}

	Control::~Control() {}

	// Return true if the input message is expected and processed
	// This function is only accessible within the GUI framework inside.
	bool Control::processMessage(const Message & message) {
		return false;
	}

	//////////////////////
	// ControlContainer //
	//////////////////////

	ControlContainer::ControlContainer() {}

	ControlContainer::~ControlContainer() {

		// Destroy all subsequent controls
		m_children.clear();
	}

	// Add a child control
	void ControlContainer::Add(Control::Shared control) {
		auto i = std::find(m_children.begin(), m_children.end(), control);
		if (i != m_children.end()) return;

		m_children.push_back(control);
	}

	// Remove a child control
	void ControlContainer::Remove(Control::Shared control) {
		auto i = std::find(m_children.begin(), m_children.end(), control);
		if (i != m_children.end()) m_children.erase(i);
	}

	// Return true if the input message is expected and processed
	// This function is only accessible within the GUI framework inside.
	bool ControlContainer::processMessage(const Message & message) {

		bool result = false;

		for (auto i = m_children.begin(); i != m_children.end(); ++i) {
			result = (*i)->processMessage(message);
			if (result) return true;
		}

		return Control::processMessage(message);
	}

	////////////////
	// WindowHost //
	////////////////

	WindowHost::WindowHost() {}

	WindowHost::~WindowHost() {}

	////////////
	// Window //
	////////////

	Window::Window() {
	}

	Window::~Window() {
	}

	void Window::initializeHost() {

		// Avoid multiple initialization
		if (m_host)
			return;

		// Create the OS-specific top level window and attach to it
		m_host = App::createWindowHost();
		m_host->OnClose += EVENTHANDLER_FROM_THIS(Window::doOnHostClose);

		// Other initializations
		// ...
	}

	void Window::deinitializeHost() {

		// Other deinitialization
		// ...

		// Release host
		m_host.reset();
	}

	void Window::show() {

		initializeHost();

#ifdef V2X_WINDOWS
		WindowHostWin::Shared h = std::dynamic_pointer_cast<WindowHostWin>(m_host);
		if (!h) 
			throw Exception(L"Window::show(): The internal window host is invalid!");
#else
#error Not implemented!
#endif

		h->show();
	}

	void Window::doOnHostClose(Event::Shared e) {

		deinitializeHost();
	}
}
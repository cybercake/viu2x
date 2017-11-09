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

	///////////////////////////
	// EventDataWindowSize //
	///////////////////////////

	EventDataWindowSize::EventDataWindowSize(
		const WindowState & state, const Vector2D64F & position, const Size2D64F & size) :
		State(state), Position(position), Size(size) {}

	EventDataWindowSize::~EventDataWindowSize() {}

	////////////////////
	// EventDataMouse //
	////////////////////

	EventDataMouse::EventDataMouse(const Vector2D64F position,
		const EnumSet<MouseButton> & buttons,
		const EnumSet<KeyModifier> & modifiers) :
		Position(position), Buttons(buttons), Modifiers(modifiers) {}

	EventDataMouse::EventDataMouse(const Vector2D64F position,
		const MouseButton & button,
		const EnumSet<KeyModifier> & modifiers) :
		Position(position), Buttons(button), Modifiers(modifiers) {}

	EventDataMouse::~EventDataMouse() {}

	///////////////////////
	// EventDataKeyboard //
	///////////////////////

	EventDataKeyboard::EventDataKeyboard(const String & key,
		const EnumSet<KeyModifier> & modifiers) : 
		Key(key), Modifiers(modifiers) {}

	EventDataKeyboard::EventDataKeyboard(const KeyModifier & modifier) :
		Key('\0'), Modifiers(modifier) {}

	EventDataKeyboard::~EventDataKeyboard() {}

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
		deinitializeHost();
	}

	void Window::initializeHost() {

		// Avoid multiple initialization
		if (m_host)
			return;

		// Create the OS-specific top level window and attach to it
		m_host = App::createWindowHost();
		m_host->OnShow += EVENTHANDLER_FROM_THIS(Window::doOnHostShow);
		m_host->OnClose += EVENTHANDLER_FROM_THIS(Window::doOnHostClose);
		m_host->OnResize += EVENTHANDLER_FROM_THIS(Window::doOnHostResize);

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
		h->show();
#else
#error Not implemented!
#endif
	}

	void Window::close() {
#ifdef V2X_WINDOWS
		WindowHostWin::Shared h = std::dynamic_pointer_cast<WindowHostWin>(m_host);
		if (!h)
			throw Exception(L"Window::close(): The internal window host is invalid!");
		h->close();
#else
#error Not implemented!
#endif
	}

	void Window::doOnHostShow(Event::Shared e) {

		auto data = e->getDataAs<const EventDataWindowSize>();
		if (!data)
			throw Exception(L"Window::doOnHostShow(): The event data type is unexpected!");

	}

	void Window::doOnHostClose(Event::Shared e) {
	}

	void Window::doOnHostResize(Event::Shared e) {

		auto data = e->getDataAs<const EventDataWindowSize>();
		if (!data)
			throw Exception(L"Window::doOnHostResize(): The event data type is unexpected!");


	}
}
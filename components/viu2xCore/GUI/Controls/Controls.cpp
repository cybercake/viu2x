/* Copyright (C) Hao Qin. All rights reserved. */

#include "../../common.h"

#include "../Displays.h"
#include "Controls.h"
#include "App.h"

namespace v2x {

	/////////////
	// Control //
	/////////////

	Control::Control() :
		Layout(LISTENER(this, Control::doOnLayoutChange)),
		Font(LISTENER(this, Control::doOnFontChange)), 
		Cursor(LISTENER(this, Control::doOnCursorChange)) {}

	Control::~Control() {}

	// Return true if the input message is expected and processed
	// This function is only accessible within the GUI framework inside.
	bool Control::processMessage(const Message & message) {
		return false;
	}

	void Control::invalidateLayout() {}

	void Control::invalidateCanvas() {}

	void Control::doOnLayoutChange(const void * sender, const void * data) {}

	void Control::doOnFontChange(const void * sender, const void * data) {}

	void Control::doOnCursorChange(const void * sender, const void * data) {}

	//////////////////////
	// ControlContainer //
	//////////////////////

	ControlContainer::ControlContainer() :
		ContentLayout(LISTENER(this, ControlContainer::doOnContentLayoutChange)) {}

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

	void ControlContainer::doOnContentLayoutChange(const void * sender, const void * data) {
		invalidateLayout();
		invalidateCanvas();
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

		// Initialize default window size as 1/3 of the screen size.
		Displays displays;
		Layout.Width.Size = displays.getPrimaryDisplay()->getScreenAreaInPx().getWidth() / 3;
		Layout.Height.Size = displays.getPrimaryDisplay()->getScreenAreaInPx().getHeight() / 3;
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
		if (Layout.Width.Size.isSet() || Layout.Height.Size.isSet()) {
			Size2D64F defaultSize = m_host->getDefaultWindowSize();
			double w = Layout.Width.Size.isSet() ? Layout.Width.Size.get() : defaultSize.width;
			double h = Layout.Height.Size.isSet() ? Layout.Height.Size.get() : defaultSize.height;
			m_host->setPosition(Rect64F(0, 0, w, h));
		}
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

		if (!m_host)
			throw Exception(L"Window::show(): The internal window host is not initialized!");
		m_host->show();
	}

	void Window::close() {
		if (!m_host)
			throw Exception(L"Window::close(): The internal window host is not initialized!");
		m_host->close();
	}

	double Window::getActualLeft() const { return m_actualPosition.getLeft(); }
	double Window::getActualTop() const { return m_actualPosition.getTop(); }
	double Window::getActualWidth() const { return m_actualPosition.getWidth(); }
	double Window::getActualHeight() const { return m_actualPosition.getHeight(); }

	void Window::doOnHostShow(Event::Shared e) {

		auto data = e->getDataAs<const EventDataWindowSize>();

	}

	void Window::doOnHostClose(Event::Shared e) {
	}

	void Window::doOnHostResize(Event::Shared e) {

		auto data = e->getDataAs<const EventDataWindowSize>();

		Rect64F newRect(data->Position.x, data->Position.y, data->Size.width, data->Size.height);
		bool sizeChanged = newRect.size != m_actualPosition.size;
		m_actualPosition = newRect;

		if (sizeChanged)
			invalidateLayout();
	}
}
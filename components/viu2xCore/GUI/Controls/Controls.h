/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"
#include "../Graphics/Layout.h"
#include "WindowHost.h"

namespace v2x {

	/// The mouse cursor specification
	///
	enum class Cursor {
		Arrow,
		Busy
	};
	/// The strings for MouseButton
	std::vector<const Char *> EnumString<Cursor>::m_strings = {
		L"Arrow",
		L"Busy"
	};
	typedef SimpleSpec<Cursor> CursorSpec;

	// The common class for all visual elements
	//
	class Control : public Object, public MessageHandler {
		friend class ControlContainer;
	public:
		DEFINE_POINTERS(Control);

		Control();

		virtual ~Control();

		virtual void show() = 0;
		virtual void close() = 0;

		LayoutSpec Layout;
		FontSpec Font;

		CursorSpec Cursor;

		// Mouse events..
		// MouseMove
		// MouseClick
		// MouseEnter
		// MouseLeave
		// MouseDoubleClick
		// MouseDown
		// MouseUp
		// MouseDrag

		// Keyboard events
		// KeyDown
		// KeyUp
		// KeyStoke
		// GetFocus
		// LoseFocus

	protected:
		// Return true if the input message is expected and processed
		// This function is only accessible within the GUI framework inside.
		bool processMessage(const Message & message) override;

		virtual void invalidateLayout();
		virtual void invalidateCanvas();

		virtual void doOnLayoutChange(const void * sender, const void * data);
		virtual void doOnFontChange(const void * sender, const void * data);
		virtual void doOnCursorChange(const void * sender, const void * data);
	};

	/// This class is the common base for the controls with subsequent controls.
	///
	/// It forward the incoming messages to the children controls.
	///
	class ControlContainer : public Control {

	public:
		DEFINE_POINTERS(ControlContainer);

		ControlContainer();

		virtual ~ControlContainer();

		// Add a child control
		void Add(Control::Shared control);
		// Remove a child control
		void Remove(Control::Shared control);

		ContentLayoutSpec ContentLayout;

	protected:

		typedef std::vector<Control::Shared> SubControls;

		// Owned reference to child contorls
		SubControls m_children;

		bool processMessage(const Message & message) override;

		virtual void doOnContentLayoutChange(const void * sender, const void * data);
	};

	/// The visual state of a window
	enum class WindowState {
		/// A floating window on the screen with fixed size
		Normal,

		/// A temporarily hidden window
		Minimized,

		/// A window whose borders are sticked to the screen edges
		Maximized
	};
	/// The strings for WindowState
	std::vector<const Char *> EnumString<WindowState>::m_strings = {
		L"Normal",
		L"Minimized",
		L"Maximized"
	};

	/// This event data represents the resizing behaviour of a window.
	///
	class EventDataWindowSize : public Object {
	public:
		DEFINE_POINTERS(EventDataWindowSize);

		EventDataWindowSize(const WindowState & state, const Vector2D64F & position, const Size2D64F & size);
		~EventDataWindowSize();

		WindowState State;
		Vector2D64F Position;
		Size2D64F Size;
	};

	/// The basic mouse buttons
	enum class MouseButton {
		Left,
		Mittle,
		Right
	};
	/// The strings for MouseButton
	std::vector<const Char *> EnumString<MouseButton>::m_strings = {
		L"Left",
		L"Middle",
		L"Right"
	};

	/// The basic key modifiers
	enum class KeyModifier {
		Control,
		Alt,
		Shift,
		Command
	};
	/// The strings for KeyModifier
	std::vector<const Char *> EnumString<KeyModifier>::m_strings = {
		L"Control",
		L"Alt",
		L"Shift",
		L"Command"
	};

	/// This event data represents mouse moving and mouse button hitting events.
	/// 
	/// The Position field represents the relative mouse cursor position.
	///
	/// The Buttons field will always show the mouse button state related to 
	/// the event.
	///
	/// The Modifiers field will always show the state related to the event.
	///
	class EventDataMouse : public Object {
	public:
		DEFINE_POINTERS(EventDataMouse);

		EventDataMouse(const Vector2D64F position,
			const EnumSet<MouseButton> & buttons,
			const EnumSet<KeyModifier> & modifiers);
		EventDataMouse(const Vector2D64F position,
			const MouseButton & button,
			const EnumSet<KeyModifier> & modifiers);
		~EventDataMouse();

		Vector2D64F Position;
		EnumSet<MouseButton> Buttons;
		EnumSet<KeyModifier> Modifiers;
	};

	/// This event data represents a key hitting event.
	/// 
	/// It can even represent the modifier key down/up events. In this case, 
	/// the member Key will be set to the corresponding modifier string. See
	/// EnumString<KeyModifier> for more information.
	///
	/// In case of special key stroke, the Key field holds the corresponding 
	/// string of that key, which is usually longer than one character. For
	/// example, the Key field will be set to L"F10" when the key F10 is 
	/// pressed.
	///
	/// The Modifiers field will always show the state related to the event.
	///
	class EventDataKeyboard : public Object {
	public:
		DEFINE_POINTERS(EventDataKeyboard);

		EventDataKeyboard(const String & key,
			const EnumSet<KeyModifier> & modifiers);
		EventDataKeyboard(const KeyModifier & modifier);
		~EventDataKeyboard();

		String Key;
		EnumSet<KeyModifier> Modifiers;
	};

	/// This class is a logical window
	///
	/// It communicate with OS through the OS-specific window host object.
	///
	class Window : public ControlContainer {

	public:
		DEFINE_POINTERS(Window);

		Window();

		virtual ~Window();

		void show() override;
		void close() override;

		/// Returns the actual window width in device unit [px]
		double getActualLeft() const;
		/// Returns the actual window height in device unit [px]
		double getActualTop() const;
		/// Returns the actual window left position in device unit [px]
		double getActualWidth() const;
		/// Returns the actual window top position in device unit [px]
		double getActualHeight() const;

	protected:
		virtual void doOnHostShow(Event::Shared e);
		virtual void doOnHostClose(Event::Shared e);
		virtual void doOnHostResize(Event::Shared e);

	private:

		WindowHost::Shared m_host;
		Rect64F m_actualPosition;

		/// This function will be called after the construction.
		void initializeHost();
		/// This function is called after the host window is closed.
		void deinitializeHost();
	};
}
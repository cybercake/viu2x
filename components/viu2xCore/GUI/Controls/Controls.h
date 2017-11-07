/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"

namespace v2x {

	// The common class for all visual elements
	//
	class Control : public Object, public MessageHandler {
		friend class ControlContainer;
	public:
		DEFINE_POINTERS(Control);

		Control();

		virtual ~Control();

		virtual void show() = 0;

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

	protected:

		typedef std::vector<Control::Shared> SubControls;

		// Owned reference to child contorls
		SubControls m_children;

		bool processMessage(const Message & message) override;
	};

	/// This class is a physical window
	///
	/// More details will be exposed by the OS-specific descendant class.
	///
	class WindowHost : public Object {
	public:
		DEFINE_POINTERS(WindowHost);

		WindowHost();

		virtual ~WindowHost();

		EventSlot OnClose;
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

	protected:
		virtual void doOnHostClose(Event::Shared e);

	private:

		WindowHost::Shared m_host;

		/// This function will be called after the construction.
		void initializeHost();
		/// This function is called after the host window is closed.
		void deinitializeHost();
	};
}
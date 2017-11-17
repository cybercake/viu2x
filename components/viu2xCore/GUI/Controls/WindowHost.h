/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"

namespace v2x {

	/// This class is the general interface to a physical window in the actual
	/// OS. It generalize the following things:
	/// - Window state changes (showing/closing/resizing/...)
	/// - User inputs (mouse/keys)
	/// - Painting
	///
	class WindowHost : public Object {
	public:
		DEFINE_POINTERS(WindowHost);

		WindowHost();

		virtual ~WindowHost();

		virtual void show() = 0;
		virtual void close() = 0;
		virtual void setPosition(const Rect64F & position) = 0;
		
		/// This function returns the default window size of the v2x system.
		virtual Size2D64F getDefaultWindowSize() = 0;

		EventSlot OnShow;
		EventSlot OnClose;
		EventSlot OnResize;

		EventSlot OnMouseMove;
		EventSlot OnMouseButtonDown;
		EventSlot OnMouseButtonUp;

		EventSlot OnKeyDown;
		EventSlot OnKeyUp;
		EventSlot OnKeyStroke;

		EventSlot OnPaint;
	};

}
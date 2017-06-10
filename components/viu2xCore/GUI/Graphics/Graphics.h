/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"

namespace v2x {

	// An abstract graphic interface. It could have different implementations 
	// e.g. GDI, GDI+, OpenGL, ...
	// 
	// This class has little idea to layouts. It works as a low level interface.
	class Canvas : public Object
	{
	public:
		DEFINE_POINTERS(Canvas);

		virtual ~Canvas();

		virtual void push(const Transformation2D & transformation) = 0;
		virtual Transformation2D pop() = 0;

		virtual void moveTo(const Vector2D & p) = 0;
		virtual void lineTo(const Vector2D & p) = 0;
		virtual void drawLine(const Vector2D & from, const Vector2D & to) = 0;

		virtual void drawArc(const Vector2D & from, const Vector2D & to, const double & radius) = 0;
		virtual void drawArc(const Vector2D & center, const double & radius, const double & fromAngle, const double & toAngle) = 0;

		virtual void drawEllipse(const Rect & rect) = 0;
		virtual void drawRect(const Rect & rect) = 0;
		virtual void drawRoundedRect(const Rect & rect, const double & cornerRadius) = 0;
		virtual void fillRect(const Rect & rect) = 0;

		virtual void drawText(const Vector2D & p, const String & s);
		virtual void drawText(const Rect & rect, const String & s);
	};

}
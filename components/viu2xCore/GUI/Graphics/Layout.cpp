/* Copyright (C) Hao Qin. All rights reserved. */

#include "Layout.h"

namespace v2x {

	////////////////
	// ScalarSpec //
	////////////////

	ScalarSpec::ScalarSpec(const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(0, m_subListener), Unit(ScalarUnit::Pixel, m_subListener) {}

	ScalarSpec::ScalarSpec(const ScalarSpec & sizeSpec, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(sizeSpec.Size, m_subListener), Unit(sizeSpec.Unit, m_subListener) {}

	ScalarSpec::ScalarSpec(const double & size, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(size, m_subListener), Unit(ScalarUnit::Pixel, m_subListener) {}

	ScalarSpec::ScalarSpec(const double & size, const ScalarUnit & unit, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(size, m_subListener), Unit(unit, m_subListener) {}

	ScalarSpec::~ScalarSpec() {}

	ScalarSpec & ScalarSpec::operator = (const ScalarSpec & value) {
		beginUpdate();
		Size = value.Size;
		Unit = value.Unit;
		endUpdate();
		return *this;
	}

	bool ScalarSpec::operator == (const ScalarSpec & value) const { return Size == value.Size && Unit == value.Unit; }
	bool ScalarSpec::operator != (const ScalarSpec & value) const { return Size != value.Size || Unit != value.Unit; }

	//////////////////
	// Vector2DSpec //
	//////////////////

	Vector2DSpec::Vector2DSpec(const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(0, m_subListener), Y(0, m_subListener), Unit(ScalarUnit::Pixel, m_subListener) {}
	Vector2DSpec::Vector2DSpec(const Vector2DSpec & vector2DSpec, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(vector2DSpec.X, m_subListener), Y(vector2DSpec.Y, m_subListener), Unit(vector2DSpec.Unit, m_subListener) {}
	Vector2DSpec::Vector2DSpec(const double & x, const double & y, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(x, m_subListener), Y(y, m_subListener), Unit(ScalarUnit::Pixel, m_subListener) {}
	Vector2DSpec::Vector2DSpec(const double & x, const double & y, const ScalarUnit & unit, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(x, m_subListener), Y(y, m_subListener), Unit(unit, m_subListener) {}
	Vector2DSpec::~Vector2DSpec() {}

	Vector2DSpec & Vector2DSpec::operator = (const Vector2DSpec & value) {
		beginUpdate();
		X = value.X;
		Y = value.Y;
		Unit = value.Unit;
		endUpdate();
		return *this;
	}
	bool Vector2DSpec::operator == (const Vector2DSpec & value) const {
		return X == value.X && Y == value.Y && Unit == value.Unit;
	}
	bool Vector2DSpec::operator != (const Vector2DSpec & value) const {
		return X != value.X || Y != value.Y || Unit != value.Unit;
	}

	////////////
	// Margin //
	////////////

	MarginSpec::MarginSpec(const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(0, m_subListener), Top(0, m_subListener), Right(0, m_subListener), Bottom(0, m_subListener), Unit(ScalarUnit::Pixel, m_subListener) {}
	MarginSpec::MarginSpec(const MarginSpec & marginSpec, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(marginSpec.Left, m_subListener), Top(marginSpec.Top, m_subListener), Right(marginSpec.Right, m_subListener), Bottom(marginSpec.Bottom, m_subListener), Unit(marginSpec.Unit, m_subListener) {}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_subListener), Top(top, m_subListener), Right(right, m_subListener), Bottom(bottom, m_subListener), Unit(ScalarUnit::Pixel, m_subListener) {}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const ScalarUnit & unit, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_subListener), Top(top, m_subListener), Right(right, m_subListener), Bottom(bottom, m_subListener), Unit(unit, m_subListener) {}
	MarginSpec::~MarginSpec() {}

	MarginSpec & MarginSpec::operator = (const MarginSpec & value) {
		beginUpdate();
		Left = value.Left;
		Top = value.Top;
		Right = value.Right;
		Bottom = value.Bottom;
		Unit = value.Unit;
		endUpdate();
		return *this;
	}
	bool MarginSpec::operator == (const MarginSpec & value) const {
		return Left == value.Left &&
			Top == value.Top &&
			Right == value.Right &&
			Bottom == value.Bottom &&
			Unit == value.Unit;
	}
	bool MarginSpec::operator != (const MarginSpec & value) const {
		return Left != value.Left ||
			Top != value.Top ||
			Right != value.Right ||
			Bottom != value.Bottom ||
			Unit != value.Unit;
	}
}
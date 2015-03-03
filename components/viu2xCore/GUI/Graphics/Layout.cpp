/* Copyright (C) Hao Qin. All rights reserved. */

#include "Layout.h"

namespace v2x {

	////////////////
	// ScalarSpec //
	////////////////

	ScalarSpec::ScalarSpec(const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(m_subListener), Unit(m_subListener) {}

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
		Notifier<Vector2DSpec>(listener), X(m_subListener), Y(m_subListener), Unit(m_subListener) {}
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

	////////////////
	// MarginSpec //
	////////////////

	MarginSpec::MarginSpec(const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(m_subListener), Top(m_subListener), Right(m_subListener), Bottom(m_subListener) {}
	MarginSpec::MarginSpec(const MarginSpec & marginSpec, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(marginSpec.Left, m_subListener), Top(marginSpec.Top, m_subListener), Right(marginSpec.Right, m_subListener), Bottom(marginSpec.Bottom, m_subListener) {}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_subListener), Top(top, m_subListener), Right(right, m_subListener), Bottom(bottom, m_subListener){}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const ScalarUnit & unit, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_subListener), Top(top, m_subListener), Right(right, m_subListener), Bottom(bottom, m_subListener){}
	MarginSpec::~MarginSpec() {}

	MarginSpec & MarginSpec::operator = (const MarginSpec & value) {
		beginUpdate();
		Left = value.Left;
		Top = value.Top;
		Right = value.Right;
		Bottom = value.Bottom;
		endUpdate();
		return *this;
	}
	bool MarginSpec::operator == (const MarginSpec & value) const {
		return Left == value.Left &&
			Top == value.Top &&
			Right == value.Right &&
			Bottom == value.Bottom;
	}
	bool MarginSpec::operator != (const MarginSpec & value) const {
		return Left != value.Left ||
			Top != value.Top ||
			Right != value.Right ||
			Bottom != value.Bottom;
	}

	////////////////
	// LayoutSpec //
	////////////////

	LayoutSpec::LayoutSpec(const Listener & listener) :
		Notifier<LayoutSpec>(listener), //
		Width(m_subListener), Height(m_subListener), //
		MinWidth(m_subListener), MinHeight(m_subListener), //
		MaxWidth(m_subListener), MaxHeight(m_subListener), //
		Margin(m_subListener), PositionMode(m_subListener), //
		HorizontalAlignment(m_subListener), VerticalAlignment(m_subListener) {}
	LayoutSpec::LayoutSpec(const LayoutSpec & layoutSpec, const Listener & listener) :
		Notifier<LayoutSpec>(listener), //
		Width(layoutSpec.Width, m_subListener), Height(layoutSpec.Height, m_subListener), //
		MinWidth(layoutSpec.MinWidth, m_subListener), MinHeight(layoutSpec.MinHeight, m_subListener), //
		MaxWidth(layoutSpec.MaxWidth, m_subListener), MaxHeight(layoutSpec.MaxHeight, m_subListener), //
		Margin(layoutSpec.Margin, m_subListener), PositionMode(layoutSpec.PositionMode, m_subListener), //
		HorizontalAlignment(layoutSpec.HorizontalAlignment, m_subListener), VerticalAlignment(layoutSpec.VerticalAlignment, m_subListener) {}
	LayoutSpec::~LayoutSpec() {}

	LayoutSpec & LayoutSpec::operator = (const LayoutSpec & value) {
		beginUpdate();
		Width = value.Width;
		Height = value.Height;
		MinWidth = value.MinWidth;
		MinHeight = value.MinHeight;
		MaxWidth = value.MaxWidth;
		MaxHeight = value.MaxHeight;
		Margin = value.Margin;
		PositionMode = value.PositionMode;
		HorizontalAlignment = value.HorizontalAlignment;
		VerticalAlignment = value.VerticalAlignment;
		endUpdate();
		return *this;
	}
	bool LayoutSpec::operator == (const LayoutSpec & value) const {
		return Width == value.Width &&
			Height == value.Height &&
			MinWidth == value.MinWidth &&
			MinHeight == value.MinHeight &&
			MaxWidth == value.MaxWidth &&
			MaxHeight == value.MaxHeight &&
			Margin == value.Margin &&
			PositionMode == value.PositionMode &&
			HorizontalAlignment == value.HorizontalAlignment &&
			VerticalAlignment == value.VerticalAlignment;
	}
	bool LayoutSpec::operator != (const LayoutSpec & value) const {
		return Width != value.Width ||
			Height != value.Height ||
			MinWidth != value.MinWidth ||
			MinHeight != value.MinHeight ||
			MaxWidth != value.MaxWidth ||
			MaxHeight != value.MaxHeight ||
			Margin != value.Margin ||
			PositionMode != value.PositionMode ||
			HorizontalAlignment != value.HorizontalAlignment ||
			VerticalAlignment != value.VerticalAlignment;
	}

	///////////////////////
	// ContentLayoutSpec //
	///////////////////////

	ContentLayoutSpec::ContentLayoutSpec(const Listener & listener) :
		Notifier<ContentLayoutSpec>(listener), Padding(m_subListener), FlowAlignment(m_subListener), FlowDirection(m_subListener) {}
	ContentLayoutSpec::ContentLayoutSpec(const ContentLayoutSpec & contentLayoutSpec, const Listener & listener) :
		Notifier<ContentLayoutSpec>(listener), Padding(contentLayoutSpec.Padding, m_subListener), //
		FlowAlignment(contentLayoutSpec.FlowAlignment, m_subListener), FlowDirection(contentLayoutSpec.FlowDirection, m_subListener){}
	ContentLayoutSpec::~ContentLayoutSpec() {}

	ContentLayoutSpec & ContentLayoutSpec::operator = (const ContentLayoutSpec & value) {
		beginUpdate();
		Padding = value.Padding;
		FlowAlignment = value.FlowAlignment;
		FlowDirection = value.FlowDirection;
		endUpdate();
		return *this;
	}
	bool ContentLayoutSpec::operator == (const ContentLayoutSpec & value) const {
		return Padding == value.Padding &&
			FlowAlignment == value.FlowAlignment &&
			FlowDirection == value.FlowDirection;
	}
	bool ContentLayoutSpec::operator != (const ContentLayoutSpec & value) const {
		return Padding != value.Padding ||
			FlowAlignment != value.FlowAlignment ||
			FlowDirection != value.FlowDirection;
	}

	//////////////
	// FontSpec //
	//////////////

	FontSpec::FontSpec(const Listener & listener) :
		Notifier<FontSpec>(listener), Name(m_subListener), Size(m_subListener), Styles(m_subListener) {}
	FontSpec::FontSpec(const FontSpec & fontSpec, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontSpec.Name.get(), m_subListener), Size(11, ScalarUnit::Dot, m_subListener), Styles(m_subListener) {}
	FontSpec::FontSpec(const String & fontName, const double & size, const FontStyles & styles, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontName, m_subListener), Size(size, ScalarUnit::Dot, m_subListener), Styles(styles, m_subListener) {}
	FontSpec::FontSpec(const String & fontName, const double & size, const ScalarUnit & unit, const FontStyles & styles, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontName, m_subListener), Size(size, unit, m_subListener), Styles(styles, m_subListener) {}
	FontSpec::~FontSpec() {}

	FontSpec & FontSpec::operator = (const FontSpec & value) {
		beginUpdate();
		Name = value.Name;
		Size = value.Size;
		Styles = value.Styles;
		endUpdate();
		return *this;
	}
	bool FontSpec::operator == (const FontSpec & value) const {
		return Name == value.Name &&
			Size == value.Size &&
			Styles == value.Styles;
	}
	bool FontSpec::operator != (const FontSpec & value) const {
		return Name != value.Name ||
			Size != value.Size ||
			Styles != value.Styles;
	}
}
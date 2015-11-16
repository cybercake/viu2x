/* Copyright (C) Hao Qin. All rights reserved. */

#include "Layout.h"

namespace v2x {

	////////////////
	// ScalarSpec //
	////////////////

	ScalarSpec::ScalarSpec(const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(m_memberListener), Unit(m_memberListener) {}

	ScalarSpec::ScalarSpec(const ScalarSpec & sizeSpec, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(sizeSpec.Size, m_memberListener), Unit(sizeSpec.Unit, m_memberListener) {}

	ScalarSpec::ScalarSpec(const double & size, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(size, m_memberListener), Unit(ScalarUnit::Pixel, m_memberListener) {}

	ScalarSpec::ScalarSpec(const double & size, const ScalarUnit & unit, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(size, m_memberListener), Unit(unit, m_memberListener) {}

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
		Notifier<Vector2DSpec>(listener), X(m_memberListener), Y(m_memberListener), Unit(m_memberListener) {}
	Vector2DSpec::Vector2DSpec(const Vector2DSpec & vector2DSpec, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(vector2DSpec.X, m_memberListener), Y(vector2DSpec.Y, m_memberListener), Unit(vector2DSpec.Unit, m_memberListener) {}
	Vector2DSpec::Vector2DSpec(const double & x, const double & y, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(x, m_memberListener), Y(y, m_memberListener), Unit(ScalarUnit::Pixel, m_memberListener) {}
	Vector2DSpec::Vector2DSpec(const double & x, const double & y, const ScalarUnit & unit, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(x, m_memberListener), Y(y, m_memberListener), Unit(unit, m_memberListener) {}
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
		Notifier<MarginSpec>(listener), Left(m_memberListener), Top(m_memberListener), Right(m_memberListener), Bottom(m_memberListener) {}
	MarginSpec::MarginSpec(const MarginSpec & marginSpec, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(marginSpec.Left, m_memberListener), Top(marginSpec.Top, m_memberListener), Right(marginSpec.Right, m_memberListener), Bottom(marginSpec.Bottom, m_memberListener) {}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_memberListener), Top(top, m_memberListener), Right(right, m_memberListener), Bottom(bottom, m_memberListener){}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const ScalarUnit & unit, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_memberListener), Top(top, m_memberListener), Right(right, m_memberListener), Bottom(bottom, m_memberListener){}
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
		Width(m_memberListener), Height(m_memberListener), //
		MinWidth(m_memberListener), MinHeight(m_memberListener), //
		MaxWidth(m_memberListener), MaxHeight(m_memberListener), //
		Margin(m_memberListener), PositionMode(m_memberListener), //
		HorizontalAlignment(m_memberListener), VerticalAlignment(m_memberListener) {}
	LayoutSpec::LayoutSpec(const LayoutSpec & layoutSpec, const Listener & listener) :
		Notifier<LayoutSpec>(listener), //
		Width(layoutSpec.Width, m_memberListener), Height(layoutSpec.Height, m_memberListener), //
		MinWidth(layoutSpec.MinWidth, m_memberListener), MinHeight(layoutSpec.MinHeight, m_memberListener), //
		MaxWidth(layoutSpec.MaxWidth, m_memberListener), MaxHeight(layoutSpec.MaxHeight, m_memberListener), //
		Margin(layoutSpec.Margin, m_memberListener), PositionMode(layoutSpec.PositionMode, m_memberListener), //
		HorizontalAlignment(layoutSpec.HorizontalAlignment, m_memberListener), VerticalAlignment(layoutSpec.VerticalAlignment, m_memberListener) {}
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
		Notifier<ContentLayoutSpec>(listener), Padding(m_memberListener), FlowAlignment(m_memberListener), FlowDirection(m_memberListener) {}
	ContentLayoutSpec::ContentLayoutSpec(const ContentLayoutSpec & contentLayoutSpec, const Listener & listener) :
		Notifier<ContentLayoutSpec>(listener), Padding(contentLayoutSpec.Padding, m_memberListener), //
		FlowAlignment(contentLayoutSpec.FlowAlignment, m_memberListener), FlowDirection(contentLayoutSpec.FlowDirection, m_memberListener){}
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
		Notifier<FontSpec>(listener), Name(m_memberListener), Size(m_memberListener), Styles(m_memberListener) {}
	FontSpec::FontSpec(const FontSpec & fontSpec, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontSpec.Name.get(), m_memberListener), Size(11, ScalarUnit::Dot, m_memberListener), Styles(m_memberListener) {}
	FontSpec::FontSpec(const String & fontName, const double & size, const FontStyles & styles, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontName, m_memberListener), Size(size, ScalarUnit::Dot, m_memberListener), Styles(styles, m_memberListener) {}
	FontSpec::FontSpec(const String & fontName, const double & size, const ScalarUnit & unit, const FontStyles & styles, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontName, m_memberListener), Size(size, unit, m_memberListener), Styles(styles, m_memberListener) {}
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
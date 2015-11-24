/* Copyright (C) Hao Qin. All rights reserved. */

#include "Layout.h"

namespace v2x {

	////////////////
	// ScalarSpec //
	////////////////

	ScalarSpec::ScalarSpec(const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(m_memberChangeListener), Unit(m_memberChangeListener) {}

	ScalarSpec::ScalarSpec(const ScalarSpec & sizeSpec, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(sizeSpec.Size, m_memberChangeListener), Unit(sizeSpec.Unit, m_memberChangeListener) {}

	ScalarSpec::ScalarSpec(const double & size, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(size, m_memberChangeListener), Unit(ScalarUnit::Pixel, m_memberChangeListener) {}

	ScalarSpec::ScalarSpec(const double & size, const ScalarUnit & unit, const Listener & listener) : //
		Notifier<ScalarSpec>(listener), Size(size, m_memberChangeListener), Unit(unit, m_memberChangeListener) {}

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
		Notifier<Vector2DSpec>(listener), X(m_memberChangeListener), Y(m_memberChangeListener), Unit(m_memberChangeListener) {}
	Vector2DSpec::Vector2DSpec(const Vector2DSpec & vector2DSpec, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(vector2DSpec.X, m_memberChangeListener), Y(vector2DSpec.Y, m_memberChangeListener), Unit(vector2DSpec.Unit, m_memberChangeListener) {}
	Vector2DSpec::Vector2DSpec(const double & x, const double & y, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(x, m_memberChangeListener), Y(y, m_memberChangeListener), Unit(ScalarUnit::Pixel, m_memberChangeListener) {}
	Vector2DSpec::Vector2DSpec(const double & x, const double & y, const ScalarUnit & unit, const Listener & listener) : //
		Notifier<Vector2DSpec>(listener), X(x, m_memberChangeListener), Y(y, m_memberChangeListener), Unit(unit, m_memberChangeListener) {}
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
		Notifier<MarginSpec>(listener), Left(m_memberChangeListener), Top(m_memberChangeListener), Right(m_memberChangeListener), Bottom(m_memberChangeListener) {}
	MarginSpec::MarginSpec(const MarginSpec & marginSpec, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(marginSpec.Left, m_memberChangeListener), Top(marginSpec.Top, m_memberChangeListener), Right(marginSpec.Right, m_memberChangeListener), Bottom(marginSpec.Bottom, m_memberChangeListener) {}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_memberChangeListener), Top(top, m_memberChangeListener), Right(right, m_memberChangeListener), Bottom(bottom, m_memberChangeListener){}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const ScalarUnit & unit, const Listener & listener) :
		Notifier<MarginSpec>(listener), Left(left, m_memberChangeListener), Top(top, m_memberChangeListener), Right(right, m_memberChangeListener), Bottom(bottom, m_memberChangeListener){}
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
		Width(m_memberChangeListener), Height(m_memberChangeListener), //
		MinWidth(m_memberChangeListener), MinHeight(m_memberChangeListener), //
		MaxWidth(m_memberChangeListener), MaxHeight(m_memberChangeListener), //
		Margin(m_memberChangeListener), PositionMode(m_memberChangeListener), //
		HorizontalAlignment(m_memberChangeListener), VerticalAlignment(m_memberChangeListener) {}
	LayoutSpec::LayoutSpec(const LayoutSpec & layoutSpec, const Listener & listener) :
		Notifier<LayoutSpec>(listener), //
		Width(layoutSpec.Width, m_memberChangeListener), Height(layoutSpec.Height, m_memberChangeListener), //
		MinWidth(layoutSpec.MinWidth, m_memberChangeListener), MinHeight(layoutSpec.MinHeight, m_memberChangeListener), //
		MaxWidth(layoutSpec.MaxWidth, m_memberChangeListener), MaxHeight(layoutSpec.MaxHeight, m_memberChangeListener), //
		Margin(layoutSpec.Margin, m_memberChangeListener), PositionMode(layoutSpec.PositionMode, m_memberChangeListener), //
		HorizontalAlignment(layoutSpec.HorizontalAlignment, m_memberChangeListener), VerticalAlignment(layoutSpec.VerticalAlignment, m_memberChangeListener) {}
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
		Notifier<ContentLayoutSpec>(listener), Padding(m_memberChangeListener), FlowAlignment(m_memberChangeListener), FlowDirection(m_memberChangeListener) {}
	ContentLayoutSpec::ContentLayoutSpec(const ContentLayoutSpec & contentLayoutSpec, const Listener & listener) :
		Notifier<ContentLayoutSpec>(listener), Padding(contentLayoutSpec.Padding, m_memberChangeListener), //
		FlowAlignment(contentLayoutSpec.FlowAlignment, m_memberChangeListener), FlowDirection(contentLayoutSpec.FlowDirection, m_memberChangeListener){}
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
		Notifier<FontSpec>(listener), Name(m_memberChangeListener), Size(m_memberChangeListener), Styles(m_memberChangeListener) {}
	FontSpec::FontSpec(const FontSpec & fontSpec, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontSpec.Name.get(), m_memberChangeListener), Size(11, ScalarUnit::Dot, m_memberChangeListener), Styles(m_memberChangeListener) {}
	FontSpec::FontSpec(const String & fontName, const double & size, const FontStyles & styles, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontName, m_memberChangeListener), Size(size, ScalarUnit::Dot, m_memberChangeListener), Styles(styles, m_memberChangeListener) {}
	FontSpec::FontSpec(const String & fontName, const double & size, const ScalarUnit & unit, const FontStyles & styles, const Listener & listener) :
		Notifier<FontSpec>(listener), Name(fontName, m_memberChangeListener), Size(size, unit, m_memberChangeListener), Styles(styles, m_memberChangeListener) {}
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
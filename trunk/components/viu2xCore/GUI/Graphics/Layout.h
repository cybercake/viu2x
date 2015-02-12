/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <unordered_set>

#include "../../common.h"

namespace v2x {

	/// Alignment options in the horizontal direction
	enum class HorizontalAlignment {

		/// In stretching mode, the width of the control will be fully deceided 
		/// by the parent size and the margin settings.
		Stretch,

		/// In this mode the width of the control will be fixed as specified.
		Left,

		/// In this mode the width of the control will be fixed as specified.
		Middle,

		/// In this mode the width of the control will be fixed as specified.
		Right,
	};

	typedef SimpleSpec<HorizontalAlignment> HorizontalAlignmentSpec;

	/// Alignment options in the vertical direction
	enum class VerticalAlignment {

		/// In stretching mode, the height of the control will be fully 
		/// deceided by the parent size and the margin settings.
		Stretch,

		/// In this mode the height of the control will be fixed as specified.
		Top,

		/// In this mode the height of the control will be fixed as specified.
		Middle,

		/// In this mode the height of the control will be fixed as specified.
		Bottom,
	};

	typedef SimpleSpec<VerticalAlignment> VerticalAlignmentSpec;

	/// Alignment options in the horizontal direction
	enum class TextAlignment {

		/// All text aligned to left without adjusting the space size.
		Left,

		/// All text aligned to center without adjusting the space size.
		Center,

		/// All text aligned to right without adjusting the space size.
		Right,

		/// All lines except the last one aligned to both sides and the last 
		/// one aligned to left.
		JustifyLeft,

		/// All lines except the last one aligned to both sides and the last 
		/// one aligned to center.
		JustifyCenter,

		/// All lines except the last one aligned to both sides and the last 
		/// one aligned to right.
		JustifyRight,

		/// All lines including the last one aligned to both sides.
		JustifyBoth,
	};

	typedef SimpleSpec<TextAlignment> TextAlignmentSpec;

	/// Horizontal flowing direction of multiple sub-controls
	enum class HorizontalFlowDirection {
		LeftToRight,
		RightToLeft,
	};

	typedef SimpleSpec<HorizontalFlowDirection> HorizontalFlowDirectionSpec;

	/// Vertical flowing direction of multiple sub-controls
	enum class VerticalFlowDirection {
		TopToBottom,
		BottomToTop,
	};

	typedef SimpleSpec<VerticalFlowDirection> VerticalFlowDirectionSpec;

	/// Unit of size values.
	enum class ScalarUnit {

		/// Size strictly specified in px
		Pixel,

		/// Size strictly specified in dot
		Dot,

		/// Size strictly specified in mm
		Millimeter,

		/// Size specified relatively to parent container
		Parent,

		/// Size specified relatively to default specification (e.g. for fonts)
		Relative,
	};

	typedef SimpleSpec<ScalarUnit> ScalarUnitSpec;

	class ScalarSpec : public Specification, public Notifier < ScalarSpec > {
	public:
		ScalarSpec(const Listener & listener = nullptr);
		ScalarSpec(const ScalarSpec & sizeSpec, const Listener & listener = nullptr);
		ScalarSpec(const double & size, const Listener & listener = nullptr);
		ScalarSpec(const double & size, const ScalarUnit & unit, const Listener & listener = nullptr);
		virtual ~ScalarSpec();

		NumberSpec Size;
		ScalarUnitSpec Unit;

		ScalarSpec & operator = (const ScalarSpec & value);
		bool operator == (const ScalarSpec & value) const;
		bool operator != (const ScalarSpec & value) const;
	};

	/// A specification of control sizes consisting of a value and a unit.
	/// A NAN value by the size means to inherit the settings from the default 
	/// settings.
	/// This class should be used as a value-type.
	typedef ScalarSpec SizeSpec;

	class Vector2DSpec : public Specification, public Notifier < Vector2DSpec > {

	public:
		Vector2DSpec(const Listener & listener = nullptr);
		Vector2DSpec(const Vector2DSpec & vector2DSpec, const Listener & listener = nullptr);
		Vector2DSpec(const double & x, const double & y, const Listener & listener = nullptr);
		Vector2DSpec(const double & x, const double & y, const ScalarUnit & unit, const Listener & listener = nullptr);
		virtual ~Vector2DSpec();

		NumberSpec X;
		NumberSpec Y;
		ScalarUnitSpec Unit;

		Vector2DSpec & operator = (const Vector2DSpec & value);
		bool operator == (const Vector2DSpec & value) const;
		bool operator != (const Vector2DSpec & value) const;
	};

	typedef Vector2DSpec PointSpec;

	/// Specification of the distance between the edges of the control and the 
	/// parent.
	/// A NAN value by the size means to inherit the settings from the default 
	/// settings.
	/// This class should be used as a value-type.
	class MarginSpec : public Specification, public Notifier < MarginSpec > {

	public:
		MarginSpec(const Listener & listener = nullptr);
		MarginSpec(const MarginSpec & marginSpec, const Listener & listener = nullptr);
		MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const Listener & listener = nullptr);
		MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const ScalarUnit & unit, const Listener & listener = nullptr);
		virtual ~MarginSpec();

		NumberSpec Left;
		NumberSpec Top;
		NumberSpec Right;
		NumberSpec Bottom;
		ScalarUnitSpec Unit;

		MarginSpec & operator = (const MarginSpec & value);
		bool operator == (const MarginSpec & value) const;
		bool operator != (const MarginSpec & value) const;
	};

	enum class FontDecoration {
		Bold,
		Italic,
		Underline,
		Stroke,
	};

	typedef std::unordered_set<FontDecoration> FontDecorationSpec;

	/// Font specification which can be applied down to one single character.
	/// This class should be used as a value-type.
	class FontSpec : public Notifier < FontSpec > {

	public:
		FontSpec(const Listener & listener = nullptr);
		FontSpec(const FontSpec & fontSpec, const Listener & listener = nullptr);
		FontSpec(const String & m_fontName, const double & m_size, const FontDecorationSpec & m_decoration, const Listener & listener = nullptr);
		FontSpec(const String & m_fontName, const double & m_size, const ScalarUnit & unit, const FontDecorationSpec & m_decoration, const Listener & listener = nullptr);
		virtual ~FontSpec();

		SizeSpec Size;

		FontSpec & operator = (const FontSpec & value);
		bool operator == (const FontSpec & value) const;
		bool operator != (const FontSpec & value) const;

	protected:
		virtual void doOnSizeChgange(const SizeSpec & sender, const void * data);

	private:
		String m_name;		
		FontDecorationSpec m_decoration;
	};
}
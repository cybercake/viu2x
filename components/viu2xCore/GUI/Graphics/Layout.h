/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"

namespace v2x {

	enum class HorizontalAlignment {
		Stretch,
		Left,
		Middle,
		Right,
	};

	enum class VerticalAlignment {
		Stretch,
		Top,
		Middle,
		Bottom,
	};

	enum class HorizontalFlowDirection {
		LeftToRight,
		RightToLeft,
	};

	enum class VerticalFlowDirection {
		TopToBottom,
		BottomToTop,
	};

	enum class SizeUnit {

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

	class SizeSpec : public Notifier<SizeSpec> {
	public:
		SizeSpec(Listener<SizeSpec> notifier = nullptr);
		SizeSpec(const SizeSpec & sizeSpec, Listener<SizeSpec> notifier = nullptr);
		SizeSpec(const double & size, Listener<SizeSpec> notifier = nullptr);
		SizeSpec(const double & size, const SizeUnit & unit, Listener<SizeSpec> notifier = nullptr);
		~SizeSpec();

		const double & getSize() const;
		void setSize(const double & value);

		const SizeUnit & getUnit() const;
		void setUnit(const SizeUnit & value);

		SizeSpec & operator = (const SizeSpec & value);
		bool operator == (const SizeSpec & value) const;
		bool operator != (const SizeSpec & value) const;
		bool operator > (const SizeSpec & value) const;
		bool operator >= (const SizeSpec & value) const;
		bool operator < (const SizeSpec & value) const;
		bool operator <= (const SizeSpec & value) const;

	protected:
		void callNotify(const void * data);

	private:
		double m_size;
		SizeUnit m_unit;

		Listener<SizeSpec> m_notifier;
	};

	class MarginSpec : public Notifier<MarginSpec> {

	public:
		MarginSpec(Listener<MarginSpec> notifier = nullptr);
		MarginSpec(const MarginSpec & marginSpec, Listener<MarginSpec> notifier = nullptr);
		MarginSpec(const double & left, const double & top, const double & right, const double & bottom, Listener<MarginSpec> notifier = nullptr);
		MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const SizeUnit & unit, Listener<MarginSpec> notifier = nullptr);
		~MarginSpec();

		const double & getLeft() const;
		void setLeft(const double & value);
		const double & getTop() const;
		void setTop(const double & value);
		const double & getRight() const;
		void setRight(const double & value);
		const double & getBottom() const;
		void setBottom(const double & value);

		const SizeUnit & getUnit() const;
		void setUnit(const SizeUnit & value);

		MarginSpec & operator = (const MarginSpec & value);
		bool operator == (const MarginSpec & value) const;
		bool operator != (const MarginSpec & value) const;

	protected:
		void callNotify(const void * data);

	private:
		double m_left;
		double m_top;
		double m_right;
		double m_bottom;
		SizeUnit m_unit;

		Listener<MarginSpec> m_notifier;
	};

}
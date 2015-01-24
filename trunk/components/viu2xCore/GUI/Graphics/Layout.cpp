/* Copyright (C) Hao Qin. All rights reserved. */

#include "Layout.h"

namespace v2x {

	//////////////
	// SizeSpec //
	//////////////

	SizeSpec::SizeSpec(Listener<SizeSpec> notifier) : //
		m_size(0), m_unit(SizeUnit::Pixel), m_notifier(notifier) {}

	SizeSpec::SizeSpec(const SizeSpec & sizeSpec, Listener<SizeSpec> notifier) : //
		m_size(sizeSpec.getSize()), m_unit(sizeSpec.getUnit()), m_notifier(notifier) {}

	SizeSpec::SizeSpec(const double & size, Listener<SizeSpec> notifier) : //
		m_size(size), m_unit(SizeUnit::Pixel), m_notifier(notifier) {}

	SizeSpec::SizeSpec(const double & size, const SizeUnit & unit, Listener<SizeSpec> notifier) : //
		m_size(size), m_unit(unit), m_notifier(notifier) {}

	SizeSpec::~SizeSpec() {}

	const double & SizeSpec::getSize() const { return m_size; }
	void SizeSpec::setSize(const double & value) {
		if (!(value >= 0))
			throw Exception(L"SizeSpec::setSize(): Unexpected size value: %f", value);
		m_size = value;
		notifyChange(this, &m_size);
	}

	const SizeUnit & SizeSpec::getUnit() const { return m_unit; }
	void SizeSpec::setUnit(const SizeUnit & value) {
		m_unit = value;
		notifyChange(this, &m_unit);
	}

	SizeSpec & SizeSpec::operator = (const SizeSpec & value) {
		m_size = value.getSize();
		m_unit = value.getUnit();
		if (m_notifier != nullptr)
			m_notifier(this, this);
		return *this;
	}

	bool SizeSpec::operator == (const SizeSpec & value) const { return m_size == value.getSize() && m_unit == value.getUnit(); }
	bool SizeSpec::operator != (const SizeSpec & value) const { if (std::isnan(m_size) || std::isnan(value.getSize())) return false; return m_size != value.getSize() || m_unit != value.getUnit(); }
	bool SizeSpec::operator > (const SizeSpec & value) const {
		if (std::isnan(m_size) || std::isnan(value.getSize()))
			return false;
		if (m_unit != value.getUnit())
			throw Exception(L"SizeSpec::>: Inidentical unit!");

		return m_size > value.getSize();
	}
	bool SizeSpec::operator >= (const SizeSpec & value) const {
		if (std::isnan(m_size) || std::isnan(value.getSize()))
			return false;
		if (m_unit != value.getUnit())
			throw Exception(L"SizeSpec::>=: Inidentical unit!");

		return m_size >= value.getSize();
	}
	bool SizeSpec::operator < (const SizeSpec & value) const {
		if (std::isnan(m_size) || std::isnan(value.getSize()))
			return false;
		if (m_unit != value.getUnit())
			throw Exception(L"SizeSpec::<: Inidentical unit!");

		return m_size < value.getSize();
	}
	bool SizeSpec::operator <= (const SizeSpec & value) const {
		if (std::isnan(m_size) || std::isnan(value.getSize()))
			return false;
		if (m_unit != value.getUnit())
			throw Exception(L"SizeSpec::<=: Inidentical unit!");

		return m_size <= value.getSize();
	}

	void SizeSpec::callNotify(const void * data) {
		if (m_notifier != nullptr)
			m_notifier(this, &m_unit);
	}

	////////////
	// Margin //
	////////////

	MarginSpec::MarginSpec(Listener<MarginSpec> notifier) :
		m_left(0), m_top(0), m_right(), m_bottom(0), m_notifier(notifier) {}
	MarginSpec::MarginSpec(const MarginSpec & marginSpec, Listener<MarginSpec> notifier) :
		m_left(marginSpec.getLeft()), m_top(marginSpec.getTop()), m_right(marginSpec.getRight()), m_bottom(marginSpec.getBottom()), m_unit(marginSpec.getUnit()), m_notifier(notifier) {}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, Listener<MarginSpec> notifier) :
		m_left(left), m_top(top), m_right(right), m_bottom(bottom), m_unit(SizeUnit::Pixel), m_notifier(notifier) {}
	MarginSpec::MarginSpec(const double & left, const double & top, const double & right, const double & bottom, const SizeUnit & unit, Listener<MarginSpec> notifier) :
		m_left(left), m_top(top), m_right(right), m_bottom(bottom), m_unit(unit), m_notifier(notifier) {}
	MarginSpec::~MarginSpec() {}

	const double & MarginSpec::getLeft() const { return m_left; }
	void MarginSpec::setLeft(const double & value) {
		if (!(value >= 0))
			throw Exception(L"MarginSpec::setLeft(): Unexpected size value: %f", value);
		m_left = value;
		notifyChange(this, &m_left);
	}
	const double & MarginSpec::getTop() const { return m_top; }
	void MarginSpec::setTop(const double & value) {
		if (!(value >= 0))
			throw Exception(L"MarginSpec::setTop(): Unexpected size value: %f", value);
		m_top = value;
		notifyChange(this, &m_top);
	}
	const double & MarginSpec::getRight() const { return m_right; }
	void MarginSpec::setRight(const double & value) {
		if (!(value >= 0))
			throw Exception(L"MarginSpec::setRight(): Unexpected size value: %f", value);
		m_right = value;
		notifyChange(this, &m_right);
	}
	const double & MarginSpec::getBottom() const { return m_bottom; }
	void MarginSpec::setBottom(const double & value) {
		if (!(value >= 0))
			throw Exception(L"MarginSpec::setBottom(): Unexpected size value: %f", value);
		m_bottom = value;
		notifyChange(this, &m_bottom);
	}

	const SizeUnit & MarginSpec::getUnit() const { return m_unit; }
	void MarginSpec::setUnit(const SizeUnit & value) {
		m_unit = value;
		notifyChange(this, &m_unit);
	}

	MarginSpec & MarginSpec::operator = (const MarginSpec & value) {
		m_left = value.getLeft();
		m_top = value.getTop();
		m_right = value.getRight();
		m_bottom = value.getBottom();
		m_unit = value.getUnit();
		notifyChange(this, this);
		return *this;
	}
	bool MarginSpec::operator == (const MarginSpec & value) const {
		return m_left == value.getLeft() &&
			m_top == value.getTop() &&
			m_right == value.getRight() &&
			m_bottom == value.getBottom() &&
			m_unit == value.getUnit();
	}
	bool MarginSpec::operator != (const MarginSpec & value) const {
		if (std::isnan(m_left) || std::isnan(value.getLeft()) ||
			std::isnan(m_top) || std::isnan(value.getTop()) ||
			std::isnan(m_right) || std::isnan(value.getRight()) ||
			std::isnan(m_bottom) || std::isnan(value.getBottom())) return false;

		return m_left != value.getLeft() ||
			m_top != value.getTop() ||
			m_right != value.getRight() ||
			m_bottom != value.getBottom() ||
			m_unit != value.getUnit();
	}
}
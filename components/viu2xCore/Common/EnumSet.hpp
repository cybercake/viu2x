/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

namespace v2x {

	// A light-weight collection class supporting up to 32 elements.
	// NOTICE: The maximum value of the enum type MUST be smaller than 32.
	template <typename t>
	class EnumSet {
	public:
		EnumSet() : m_bits(0) { }
		explicit EnumSet(const t & value) : m_bits(1 << static_cast <unsigned int> (value)) { }
		EnumSet(const EnumSet<t> & set) { m_bits = set.m_bits; }

		void include(t value) { m_bits |= (1 << static_cast <unsigned int> (value)); }
		void include(const EnumSet<t> & set) { m_bits |= set.m_bits; }
		EnumSet<t> operator + (t value) const { EnumSet<t> result(*this); result.include(value); return result; }
		EnumSet<t> operator + (const EnumSet<t> & set) const { EnumSet<t> result(*this); result.include(set); return result; }
		EnumSet<t> & operator += (t value) { include(value); return *this; }
		EnumSet<t> & operator += (const EnumSet<t> & set) { include(set); return *this; }

		void exclude(t value) { m_bits &= ~(1 << static_cast <unsigned int> (value)); }
		void exclude(const EnumSet<t> & set) { m_bits &= ~set.m_bits; }
		EnumSet<t> operator - (t value) const { EnumSet<t> result(*this); result.exclude(value); return result; }
		EnumSet<t> operator - (const EnumSet<t> & set) const { EnumSet<t> result(*this); result.exclude(set); return result; }
		EnumSet<t> & operator -= (t value) { exclude(value); return *this; }
		EnumSet<t> & operator -= (const EnumSet<t> & set) { exclude(set); return *this; }

		EnumSet<t> getComplement() const { EnumSet<t> result; result.m_bits = ~m_bits; return result; }

		bool intersect(const EnumSet<t> & set) { m_bits &= set.m_bits; return m_bits != 0; }
		EnumSet<t> operator * (const EnumSet<t> & set) const { EnumSet<t> result(*this); result.intersect(set); return result; }

		bool operator == (const EnumSet<t> & set) const { return m_bits == set.m_bits; }
		bool operator == (const t & value) const { return m_bits == (1 << static_cast <unsigned int> (value)); }
		bool operator != (const EnumSet<t> & set) const { return m_bits != set.m_bits; }

		bool isEmpty() const { return m_bits == 0; }
		bool contains(t value) const { return (m_bits & (1 << (unsigned int)value)) != 0; }
		bool contains(const EnumSet<t> & set) const { return (set.m_bits & ~m_bits) == 0; }
		void clear() { m_bits = 0; }

	private:
		unsigned int m_bits;
	};

}
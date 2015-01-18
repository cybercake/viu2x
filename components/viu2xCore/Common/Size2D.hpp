/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <stdint.h>
#include <sstream>
#include "Config.h"
#include "Exceptions.h"

namespace v2x {


	/// A type used to describe the size of an 2D area on display device e.g. screen,
	/// window, picture, etc.
	///
	/// Its width or height could be negative which represents the direction of the size.
	template <typename T>
	class Size2D {
	public:

		/// The horizontal size.
		T width;

		/// The vertical size.
		T height;

		virtual ~Size2D() {
		}

		/// The standard constructor setting the size to 0.
		Size2D() {
			width = 0;
			height = 0;
		}

		/// The constructor copying data from another instance.
		///
		/// @param [in]	value	The size object from which the data should be copied.
		Size2D(const Size2D <T> & value) {
			width = value.width;
			height = value.height;
		}

		/// The constructor copying data from another size 2D of another type
		///
		/// @param[in]	other	The Size2D from which the data has to be copied.
		template <typename OtherType> explicit Size2D(const Size2D <OtherType> & other) {
			width = other.width;
			height = other.height;
		}

		/// The constructor with specified width and height.
		///
		/// @param [in]	w	The width.
		/// @param [in]	h	The height.
		Size2D(T w, T h) {
			width = w;
			height = h;
		}

		/// Operator overloaded for assignment.
		///
		/// @param [in]	op	The size object from which the data should be copied.
		///
		/// @return A reference to the current 2D size object
		Size2D <T> & operator = (const Size2D <T> & op) {
			width = op.width;
			height = op.height;
			return *this;
		}

		/// Operator overloaded for addition. The current size object is not changed.
		///
		/// @param [in]	op	The size object whose width and height should be added to the current size.
		///
		/// @return A new size object.
		Size2D <T> operator + (const Size2D <T> & op) const {
			Size2D <T> result(width + op.width, height + op.height);
			return result;
		}

		/// Operator overloaded for increment. The current size object is changed.
		///
		/// @param [in]	op	The size object whose width and height should be added to the current size.
		///
		/// @return A reference to the current 2D size object.
		Size2D <T> & operator += (const Size2D <T> & op) {
			width += op.width;
			height += op.height;
			return *this;
		}

		/// Operator overloaded for negative calculation.
		///
		/// @return A new size object containing the negative value to the current one.
		Size2D <T> operator - () const {
			Size2D <T> result(-width, -height);
			return result;
		}


		/// Operator overloaded for subtraction. The current size object is not changed.
		///
		/// @param [in]	op	The size object whose width and height should be subtracted from the current size.
		///
		/// @return A new size object.
		Size2D <T> operator - (const Size2D <T> & op) const {
			Size2D <T> result(width - op.width, height - op.height);
			return result;
		}

		/// Operator overloaded for decrement. The current size object is changed.
		///
		/// @param [in]	op	The size object whose width and height should be subtracted from the current size.
		///
		/// @return A reference to the current 2D size object.
		Size2D <T> & operator -= (const Size2D <T> & op) {
			width -= op.width;
			height -= op.height;
			return *this;
		}

		/// Operator overloaded for scalar multiplication. The current size object is changed.
		///
		/// @param [in]	op	The multiplicator.
		///
		/// @return A new size object.
		Size2D <T> operator * (const T & op) const {
			Size2D <T> result(width * op, height * op);
			return result;
		}

		/// Operator overloaded for self scalar multiplication.
		///
		/// @param [in]	op	The multiplicator.
		///
		/// @return A reference to the current size object.
		Size2D <T> & operator *= (const T & op) {
			width *= op;
			height *= op;
			return *this;
		}


		/// Operator overloaded for scalar division. The current size object is changed.
		///
		/// @param [in]	op	The divisor.
		///
		/// @return A new size object.
		///
		/// @throw SmiException if the divisor is zero.
		Size2D <T> operator / (const T & op) const {

			// Check divisor.
			if (op == 0)
				throw Exception(L"Size2D::/: The divisor should not be 0!");

			Size2D <T> result(width / op, height / op);
			return result;
		}

		/// Operator overloaded for self scalar division.
		///
		/// @param [in]	op	The divisor.
		///
		/// @return A reference to the current size object.
		///
		/// @throw SmiException if the divisor is zero.
		Size2D <T> & operator /= (const T & op) {

			// Check divisor.
			if (op == 0)
				throw Exception(L"Size2D::/=: The divisor should not be 0!");

			width /= op;
			height /= op;
			return *this;
		}

		/// Operator overloaded for equality comparison.
		///
		/// @param [in]	op	The size object to be compared.
		///
		/// @return True only if their widths and heights are equal.
		bool operator == (const Size2D <T> & op) const {
			return (width == op.width) && (height == op.height);
		}

		/// Operator overloaded for inequality comparison.
		///
		/// @param [in]	op	The size object to be compared.
		///
		/// @return True only if their widths or heights are not equal.
		bool operator != (const Size2D <T> & op) const {

			if (std::isnan(width) || std::isnan(height) || std::isnan(op.width) || std::isnan(op.height))
				return false;

			return (width != op.width) || (height != op.height);
		}

		/// @return True if any of the sizes is zero.
		bool isNaN() const {
			return std::isnan(width) || std::isnan(height);
		}

		/// @return True if both sizes is zero.
		bool isZero() const {
			return (width == 0) && (height == 0);
		}

		/// @return True if the any of the width/height is 0.
		bool isInf() const {
			return std::isinf(width) || std::isinf(height);
		}

		/// @return The product of width and height
		T getArea() const {
			return width * height;
		}
	};

	template <typename t>
	inline std::basic_ostream <wchar_t> &
		operator << (std::basic_ostream <wchar_t> & os, const Size2D <t> & s) {
		return os << L"(" << s.width << L", " << s.height << L")";
	}

	typedef Size2D <int32_t> Size2D32I;
	typedef Size2D <int64_t> Size2D64I;

	typedef Size2D <float> Size2D32F;
	typedef Size2D <double> Size2D64F;
	typedef Size2D <Real> Size2DR;

}
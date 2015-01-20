/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Config.h"
#include "Exceptions.h"
#include "String.h"

namespace v2x {

	typedef double Real;

	template <typename T>
	class Vector2D_T {

	public:

		typedef T ValueType;

		/// The scaled x coordinate
		T x;

		/// The scaled y coordinate
		T y;

		/// The default constructor
		/// It sets the vector to 0.
		Vector2D_T() : x(), y() {
		}

		virtual ~Vector2D_T() {
		}

		/// The constructor copying data from another 2D vector of the same type
		/// @param[in]	other	The vector from which the data is to be copied.
		Vector2D_T(const Vector2D_T <T> & other) : x(other.x), y(other.y)  {
		}

		/// The constructor copying data from another 2D vector of another type
		/// @param[in]	other	The vector from which the data is to be copied.
		template <typename OtherType> explicit Vector2D_T(const Vector2D_T <OtherType> & other) : x(other.x), y(other.y)  {
		}

		/// The constructor with specification of its values
		/// @param [in] xValue The scaled x value of the vector
		/// @param [in] yValue The scaled y value of the vector
		Vector2D_T(T xValue, T yValue) : x(xValue), y(yValue) {
		}

		/// Set the components to zero.
		Vector2D_T <T> & zero() {
			x = 0;
			y = 0;
			return *this;
		}

		/// Set the components of the vector and set validity to true.
		void set(T xValue, T yValue) {
			x = xValue;
			y = yValue;
		}

		/// Operator overload for assignment.
		/// @param [in] op The source of the data. Its data will be copied to the current object.
		Vector2D_T <T> & operator = (const Vector2D_T <T> & op) {
			x = op.x;
			y = op.y;
			return *this;
		}

		/// operator overload for increment
		/// @param [in] op The operand
		/// @return this = this + op
		Vector2D_T <T> & operator += (const Vector2D_T <T> & op) {
			x += op.x;
			y += op.y;
			return *this;
		}

		/// operator overload for negative calculation
		/// @return result = - this
		Vector2D_T <T> operator - () const {
			Vector2D_T <T> result(-x, -y);
			return result;
		}

		/// operator overload for decrement
		/// @param [in] op The operand
		/// @return this = this - op
		Vector2D_T <T> & operator -= (const Vector2D_T <T> & op) {
			x -= op.x;
			y -= op.y;
			return *this;
		}

		/// operator overload for self scalar multiplication
		/// @param [in] op The scale factor
		/// @return this = this * op
		Vector2D_T <T> & operator *= (const T & op) {
			x *= op;
			y *= op;
			return *this;
		}

		/// operator overload for self scalar division
		/// @param [in] op The scale factor
		/// @return this = this / op
		Vector2D_T <T> & operator /= (const T & op) {

			if (op == 0)
				throw Exception(L"Vector2D_T::/=: The operator should not be 0!");

			x /= op;
			y /= op;
			return *this;
		}

		/// operator overload for addition
		/// @param [in] op The operand
		/// @return result = this + op
		Vector2D_T <T> operator + (const Vector2D_T <T> & op) const {
			return Vector2D_T <T>(x + op.x, y + op.y);
		}

		/// operator overload for subtraction
		/// @param [in] op The operand
		/// @return result = this - op
		Vector2D_T <T> operator - (const Vector2D_T <T> & op) const {
			return Vector2D_T <T>(x - op.x, y - op.y);
		}

		/// operator overload for scalar multiplication
		/// @param [in] op The scale factor
		/// @return result = this * op
		Vector2D_T <T> operator * (const T & op) const {
			return Vector2D_T <T>(x * op, y * op);
		}

		/// operator overload for scalar division
		/// @param [in] op The scale factor
		/// @return result = this / op
		Vector2D_T <T> operator / (const T & op) const {

			if (op == 0)
				throw Exception(L"Vector2D_T::/: The operator should not be 0!");

			return Vector2D_T <T>(x / op, y / op);
		}

		/// operator overload for equality comparison
		/// @param [in] op The operand
		/// @return True if this = op considering the scale factor w
		bool operator == (const Vector2D_T <T> & op) const {
			return (x == op.x) && (y == op.y);
		}

		/// operator overload for inequality comparison
		/// @param [in] op The operand
		/// @return False if this = op considering the scale factor w
		bool operator != (const Vector2D_T <T> & op) const {

			if (std::isnan(x) || std::isnan(y) ||
				std::isnan(op.x) || std::isnan(op.y))
				return false;

			return (x != op.x) || (y != op.y);
		}

		/// Retrieving the Euclidean L2 norm.
		///
		/// @return The Euclidean L2 norm of the current vector
		Real norm() const {
			return sqrt((Real)x * (Real)x + (Real)y * (Real)y);
		}

		/// Retrieving the square of the Euclidean L2 norm.
		///
		/// @return The square of the Euclidean L2 norm of the current vector
		Real normSqr() const {
			return (Real)x * (Real)x + (Real)y * (Real)y;
		}

		/// Return the normalized vector.
		///
		/// @return The normalized vector.
		///
		/// @throw Exception if w == 0.
		Vector2D_T <T> normalize() const {
			Vector2D_T <T> result;
			Real n = norm();
			result.x = (T)(x / n);
			result.y = (T)(y / n);

			return result;
		}

		/// Compute the dot product of two vectors
		/// @param [in] op The operand
		/// @return The dot product of the current vector and op (this . op)
		T dot(const Vector2D_T <T> & op) const {
			return x * op.x + y * op.y;
		}

		/// Compute the cross product of two vectors
		/// @param [in] op The operand
		/// @return The cross product of the current vector and op (this x op)
		T cross(const Vector2D_T <T> & op) const {
			return x * op.y - y * op.x;
		}

		/// A utility function indicating if the vector is 0
		/// @return True if all component of the vector is 0
		bool isZero() const {
			return x == 0 && y == 0;
		}

		/// @return True if any of the components is NaN.
		bool isNaN() const {
			return std::isnan(x) || std::isnan(y);
		}

		/// @return True if any of the components is infinity.
		bool isInf() const {
			return std::isinf(x) || std::isinf(y);
		}

		/// This function convert the current vector data to a string in the following format:
		/// "x:###, y:###, w:###" where ### denotes the corresponding value.
		///
		/// @return	a readable string representing the vector data.
		String toString() const {
			std::wostringstream ss;
			ss << L"(x:" << x << L", y:" << y << L")";
			return ss.str();
		}
	};

	typedef Vector2D_T<int32_t> Vector2D32I;
	typedef Vector2D_T<float> Vector2D32F;
	typedef Vector2D_T<double> Vector2D64F;
	typedef Vector2D_T<Real> Vector2DR;

	typedef Vector2D_T<Real> Point;
}
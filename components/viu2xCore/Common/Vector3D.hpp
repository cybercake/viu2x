/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Config.h"
#include "Exceptions.h"
#include "String.h"

namespace v2x {

	template <typename T>
	class Vector3D_T {

	public:

		typedef T ValueType;

		/// The scaled x coordinate
		T x;

		/// The scaled y coordinate
		T y;

		/// The scaled z coordinate
		T z;

		/// The default constructor
		/// It sets the vector to 0.
		Vector3D_T() : x(), y(), z() {
		}

		virtual ~Vector3D_T() {
		}

		/// The constructor copying data from another 2D vector of the same type
		/// @param[in]	other	The vector from which the data is to be copied.
		Vector3D_T(const Vector2D_T <T> & other) : x(other.x), y(other.y), z(other.z)  {
		}

		/// The constructor copying data from another 2D vector of another type
		/// @param[in]	other	The vector from which the data is to be copied.
		template <typename OtherType> explicit Vector3D_T(const Vector3D_T <OtherType> & other) : x(other.x), y(other.y), z(other.z) {
		}

		/// The constructor with specification of its values
		/// @param [in] xValue The scaled x value of the vector
		/// @param [in] yValue The scaled y value of the vector
		Vector3D_T(T xValue, T yValue, T zValue) : x(xValue), y(yValue), z(zValue) {
		}

		/// Set the components to zero.
		Vector3D_T <T> & zero() {
			x = 0;
			y = 0;
			z = 0;
			return *this;
		}

		/// Set the components of the vector and set validity to true.
		void set(T xValue, T yValue, T zValue) {
			x = xValue;
			y = yValue;
			z = zValue;
		}

		/// Operator overload for assignment.
		/// @param [in] op The source of the data. Its data will be copied to the current object.
		Vector3D_T <T> & operator = (const Vector3D_T <T> & op) {
			x = op.x;
			y = op.y;
			z = op.z;
			return *this;
		}

		/// operator overload for increment
		/// @param [in] op The operand
		/// @return this = this + op
		Vector3D_T <T> & operator += (const Vector3D_T <T> & op) {
			x += op.x;
			y += op.y;
			z += op.z;
			return *this;
		}

		/// operator overload for negative calculation
		/// @return result = - this
		Vector3D_T <T> operator - () const {
			return Vector3D_T <T>(-x, -y, -z);
		}

		/// operator overload for decrement
		/// @param [in] op The operand
		/// @return this = this - op
		Vector3D_T <T> & operator -= (const Vector3D_T <T> & op) {
			x -= op.x;
			y -= op.y;
			z -= op.z;
			return *this;
		}

		/// operator overload for self scalar multiplication
		/// @param [in] op The scale factor
		/// @return this = this * op
		Vector3D_T <T> & operator *= (const T & op) {
			x *= op;
			y *= op;
			z *= op;
			return *this;
		}

		/// operator overload for self scalar division
		/// @param [in] op The scale factor
		/// @return this = this / op
		Vector3D_T <T> & operator /= (const T & op) {

			if (op == 0)
				throw Exception(L"Vector3D_T::/=: The operator should not be 0!");

			x /= op;
			y /= op;
			z /= op;
			return *this;
		}

		/// operator overload for addition
		/// @param [in] op The operand
		/// @return result = this + op
		Vector3D_T <T> operator + (const Vector3D_T <T> & op) const {
			return Vector3D_T <T>(x + op.x, y + op.y, z + op.z);
		}

		/// operator overload for subtraction
		/// @param [in] op The operand
		/// @return result = this - op
		Vector3D_T <T> operator - (const Vector3D_T <T> & op) const {
			return Vector3D_T <T>(x - op.x, y - op.y, z - op.z);
		}

		/// operator overload for scalar multiplication
		/// @param [in] op The scale factor
		/// @return result = this * op
		Vector3D_T <T> operator * (const T & op) const {
			return Vector3D_T <T>(x * op, y * op, z * op);
		}

		/// operator overload for scalar division
		/// @param [in] op The scale factor
		/// @return result = this / op
		Vector3D_T <T> operator / (const T & op) const {

			if (op == 0)
				throw Exception(L"Vector3D_T::/: The operator should not be 0!");

			return Vector3D_T <T>(x / op, y / op, z / op);
		}

		/// operator overload for equality comparison
		/// @param [in] op The operand
		/// @return True if this = op considering the scale factor w
		bool operator == (const Vector3D_T <T> & op) const {
			return (x == op.x) && (y == op.y) && (z == op.z);
		}

		/// operator overload for inequality comparison
		/// @param [in] op The operand
		/// @return False if this = op considering the scale factor w
		bool operator != (const Vector3D_T <T> & op) const {

			if (std::isnan(x) || std::isnan(y) || std::isnan(z) ||
				std::isnan(op.x) || std::isnan(op.y) || std::isnan(op.z))
				return false;

			return (x != op.x) || (y != op.y) || (z != op.z);
		}

		/// Retrieving the Euclidean L2 norm.
		/// @return The Euclidean L2 norm of the current vector
		Real norm() const {
			return sqrt((Real)x * (Real)x + (Real)y * (Real)y + (Real)z * (Real)z);
		}

		/// Retrieving the square of the Euclidean L2 norm.
		/// @return The square of the Euclidean L2 norm of the current vector
		Real normSqr() const {
			return (Real)x * (Real)x + (Real)y * (Real)y + (Real)z * (Real)z;
		}

		/// Return the normalized vector.
		/// @return The normalized vector.
		/// @throw Exception if norm == 0.
		Vector3D_T <T> normalize() const {
			Vector3D_T <T> result;
			Real n = norm();
			result.x = (T)(x / n);
			result.y = (T)(y / n);
			result.z = (T)(z / n);

			return result;
		}

		/// Compute the dot product of two vectors
		/// @param [in] op The operand
		/// @return The dot product of the current vector and op (this . op)
		T dot(const Vector3D_T <T> & op) const {
			return x * op.x + y * op.y + z * op.z;
		}

		/// Compute the cross product of two vectors
		/// @param [in] op The operand
		/// @return The cross product of the current vector and op (this x op)
		T cross(const Vector3D_T <T> & op) const {
			Vector3D_T <T> result;
			result.x = y * op.z - z * op.y;
			result.y = z * op.x - x * op.z;
			result.z = x * op.y - y * op.x;
			return result;
		}

		/// A utility function indicating if the vector is 0
		/// @return True if all component of the vector is 0
		bool isZero() const {
			return x == 0 && y == 0 && z == 0;
		}

		/// @return True if any of the components is NaN.
		bool isNaN() const {
			return std::isnan(x) || std::isnan(y) || std::isnan(z);
		}

		/// @return True if any of the components is infinity.
		bool isInf() const {
			return std::isinf(x) || std::isinf(y) || std::isinf(z);
		}

		/// This function convert the current vector data to a string in the following format:
		/// "x:###, y:###, z:###" where ### denotes the corresponding value.
		/// @return	a readable string representing the vector data.
		String toString() const {
			std::wostringstream ss;
			ss << L"(x:" << x << L", y:" << y << L", z:" << z << L")";
			return ss.str();
		}
	};

	typedef Vector3D_T<int32_t> Vector3D32I;
	typedef Vector3D_T<float> Vector3D32F;
	typedef Vector3D_T<double> Vector3D64F;
	typedef Vector3D_T<Real> Vector3DR;

	typedef Vector3D_T<Real> Point3D;
}
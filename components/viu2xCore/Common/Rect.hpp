/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Vector2D.hpp"
#include "Size2D.hpp"

namespace v2x {

	/// A type used to describe a 2D rectangular area on display device e.g. screen, window, picture, etc.
	///
	/// It has not only position and sizes but also directions!
	///
	/// The data field "position" represents the starting point of the rectangle. It is one of the corner
	/// of the rectangle. And the data field "size" could have negative values, it represents the size and
	/// direction of the rectangle. If the width is negative, then the rectangle stands on the left side of
	/// the starting point. If the height is negative, then the rectangle stands above the starting point.
	///
	/// Convention:
	/// The rectangle is described by its top-left and bottom-right corner p1 and p2.
	/// If the rectangle is an integer rect, then the pixel p2 is assumed to be NOT part of the rectangle
	/// itself. The most bottom right pixel which is included in the rectangle is p2-(1,1). The size of the
	/// rect is the number of pixels contained in the rectangle (computed as p2-p1) in each dimension.
	/// If the rectangle is a real-valued rect, then p1 and p2 denote the exact corner points of the rectangle.
	/// The pixels on the right and bottom side are seen as NOT included in the rect. The size of the rect is
	/// the length of the horizontal and vertical side of the rectangle (also computed as p2-p1).
	template <typename T>
	class Rect_T {
	public:

		/// The starting point (i.e. one of the corner) of the rectangle.
		///
		/// NOTE: it is not always the top-left position!!!
		/// If you want to get the left/top-of-all positions, please use getLeft/getTop
		Vector2D_T <T> position;

		/// The size of the rectangle.
		///
		/// NOTE: Its value could be negative!!!
		Size2D_T <T> size;

		/// The standard constructor.
		Rect_T() {
			position = Vector2D_T <T>(0, 0);
			size = Size2D_T <T>(0, 0);
		}

		/// The constructor copying data from another instance.
		///
		/// @param [in]	rect	The rectangle from which the data should be copied.
		Rect_T(const Rect_T <T> & rect) {
			position = rect.position;
			size = rect.size;
		}

		/// The constructor with specified values.
		///
		/// @param [in]	x		The X-coordinate of the starting point.
		/// @param [in]	y		The Y-coordinate of the starting point.
		/// @param [in]	width	The width of the starting point. It could be negative.
		/// @param [in]	height	The height of the starting point. It could be negative.
		Rect_T(T x, T y, T width, T height) {
			position = Vector2D_T <T>(x, y);
			size = Size2D_T <T>(width, height);
		}

		/// The constructor with specified corner position and size.
		///
		/// @param [in]	p	The starting position of the rectangle.
		/// @param [in]	s	The size of the rectangle. Its values could be negative.
		///
		/// The scaling factor w in the starting position is NOT used in this class. So before accepting any
		/// 2D vector as starting point, it has to be regularized first!!!
		Rect_T(Vector2D_T <T> p, Size2D_T <T> s) {
			position = p;
			size = s;
		}

		/// The constructor with specified corner positions.
		///
		/// @param [in]	p1	The starting point of the rectangle.
		/// @param [in]	p2	The ending point of the rectangle. The size of the rectangle
		/// 					is just set using the difference between p1 and p2 (size = p2 - p1).
		/// 					Note: The point p2 itself is not part of the rectangle (see notes in class documentation).
		///
		/// The scaling factor w in the starting position is NOT used in this class. So before accepting any
		/// 2D vector as starting point, it has to be regularized first!!!
		Rect_T(Vector2D_T <T> p1, Vector2D_T <T> p2) {
			position = p1;
			Vector2D_T <T> d = (p2 - p1);
			size = Size2D_T <T>(d.x, d.y);
		}

		/// The constructor copying data from another 2D rect of another type
		///
		/// @param[in]	other	The rect from which the data has to be copied.
		template <typename otherType> explicit Rect_T(const Rect_T <otherType> & other) {
			position = Vector2D_T <T>(other.position);
			size = Size2D_T <T>(other.size);
		}

		/// The destructor.
		virtual ~Rect_T() {
		}

		/// Convenience function to set parameters of the current rectangle.
		///
		/// @param [in]	x		The X-coordinate of the starting point.
		/// @param [in]	y		The Y-coordinate of the starting point.
		/// @param [in]	width	The width of the starting point. It could be negative.
		/// @param [in]	height	The height of the starting point. It could be negative.
		void set(T x, T y, T width, T height) {
			position = Vector2D_T <T>(x, y);
			size = Size2D_T <T>(width, height);
		}

		/// Centers the rect around the given center point and recomputes the position
		/// while the size remains untouched.
		///
		/// @param [in] 	center	The new center vector of the rect
		void setCenter(Vector2D_T <T> center) {
			position = Vector2D_T <T>(center.x - size.width / 2, center.y - size.height / 2);
		}

		/// Convenience function to set boundaries of the current rectangle.
		/// Note: The point right/bottom itself is not part of the rectangle (see notes in class documentation).
		///
		/// @param [in]	left	The X-coordinate of the starting point.
		/// @param [in]	top		The Y-coordinate of the starting point.
		/// @param [in]	right	The X-coordinate of the ending point.
		/// @param [in]	bottom	The Y-coordinate of the ending point.
		void setBounds(T left, T top, T right, T bottom) {
			position = Vector2D_T <T>(left, top);
			size = Size2D_T <T>(right - left, bottom - top);
		}

		/// Clip the current rectangle by a given boundary. If the rectangle and the
		/// clipper does not intersect, the rectangle is set to (0, 0, 0, 0).
		///
		/// @param [in]	clipper		The boundary.
		/// @return	True if the rectangle intersect with the clipper.
		bool clipBy(const Rect_T <T>& clipper) { // TODO: CHECK THIS

			// Calculate the boundary of the clipper.
			T clipperLeft = clipper.getLeft();
			T clipperTop = clipper.getTop();
			T clipperRight = clipper.getRight();
			T clipperBottom = clipper.getBottom();

			// Calculate the boundary of the current rectangle.
			T left = getLeft();
			T top = getTop();
			T right = getRight();
			T bottom = getBottom();

			// Compute the clipped value.
			T x1 = left < clipperLeft ? clipperLeft : left;
			T y1 = top < clipperTop ? clipperTop : top;
			T x2 = right > clipperRight ? clipperRight : right;
			T y2 = bottom > clipperBottom ? clipperBottom : bottom;

			// Check if the rectangles intersect with each other.
			if (x1 > x2 || y1 > y2) {
				setBounds(0, 0, 0, 0);
				return false;
			}

			setBounds(x1, y1, x2, y2);
			return true;
		}

		/// Dilates the rectangle by the given amount.
		void dilate(T amount) {
			position.x -= amount;
			position.y -= amount;
			size.width += 2 * amount;
			size.height += 2 * amount;
		}

		/// Convenience function to check if a point is in the current rectangle.
		///
		/// @param [in]	p	A 2D point which should be checked.
		///
		/// @return True if the point is inside the rectangle
		///
		/// The scaling factor w in the starting position is NOT used in this class. So before comparing any
		/// 2D vector with the starting point, it has to be regularized first!!!
		///
		/// This method implementation is consistent with definition of rect, in that positions on the right
		/// and bottom border are considered outside of the rectangle
		const bool contains(const Vector2D32I& p) const {
			return p.y >= getTop() && p.x >= getLeft() && p.y < getBottom() && p.x < getRight();
		}

		/// Convenience function to check if a point is in the current rectangle.
		///
		/// @param [in]	p	A 2D point which should be checked.
		///
		/// @return True if the point is inside the rectangle
		///
		/// The scaling factor w in the starting position is NOT used in this class. So before comparing any
		/// 2D vector with the starting point, it has to be regularized first!!!
		///
		/// This method implementation is consistent with definition of rect, in that positions on the right
		/// and bottom border are considered outside of the rectangle
		const bool contains(const Vector2D64F& p) const {
			return p.y >= getTop() && p.x >= getLeft() && p.y < getBottom() && p.x < getRight();
		}

		template <typename U>
		const bool contains(const U x, const U y) const {
			return y >= getTop() && x >= getLeft() && y < getBottom() && x < getRight();
		}

		/// @return the left-of-all position.
		T getLeft() const {
			return size.width > 0 ? position.x : position.x + size.width;
		}

		/// @return the top-of-all position.
		T getTop() const {
			return size.height > 0 ? position.y : position.y + size.height;
		}

		/// @return the bottom-of-all position.
		/// Note: The bottom edge itself is not part of the rectangle (see notes in class documentation).
		T getBottom() const {
			return size.height > 0 ? position.y + size.height : position.y;
		}

		/// @return the right-of-all position.
		/// Note: The right edge itself is not part of the rectangle (see notes in class documentation).
		T getRight() const {
			return size.width > 0 ? position.x + size.width : position.x;
		}

		/// @return the width of the rectangle. It's always positive.
		T getWidth() const {
			return size.width >= 0 ? size.width : -size.width;
		}

		/// @return the height of the rectangle. It's always positive.
		T getHeight() const {
			return size.height >= 0 ? size.height : -size.height;
		}

		/// Get the rectangles center position
		///
		/// @return 	The calculated center vector of the rectangle with respect to the underlying type
		Vector2D_T <T> getCenter() const {
			return Vector2D_T <T>(position.x + (size.width / (T)2), position.y + (size.height / (T)2));
		}

		/// Get the rectangles center position in floating point precision
		///
		/// @return 	The calculated center vector of the rectangle
		Vector2DR getCenterExact() const {
			return Vector2DR(position.x + (size.width / (real)2), position.y + (size.height / (real)2));
		}

		/// @return if the rectangle is empty.
		bool isEmpty() const {
			return (size.width == 0) && (size.height == 0);
		}

		/// Operator overloaded for assignment.
		///
		/// @parem [in]	op	A rectangle from which the data should be copied.
		///
		/// @return A reference to the current instance.
		Rect_T <T> & operator= (const Rect_T <T> & op) {
			position = op.position;
			size = op.size;
			return *this;
		}

		/// Operator overloaded for inequality comparison.
		///
		/// @param [in]	op	The Rect_T object to be compared.
		///
		/// @return True only if size and position are equal.
		bool operator != (const Rect_T <T> & op) const {
			return (position != op.position) || (size != op.size);
		}

		/// Operator overloaded for equality comparison.
		///
		/// @param [in]	op	The Rect_T object to be compared.
		///
		/// @return True only if size and position are equal.
		bool operator == (const Rect_T <T> & op) const {
			return (position == op.position) && (size == op.size);
		}

		/// This operator adds an offset to the rectangle object and return
		/// the result.
		///
		/// @param [in]	op	The offset value.
		///
		/// @return		A new rectangle object which has the specified
		/// 				offset to the current one.
		Rect_T <T> operator + (const Vector2D_T <T> & op) const {
			return Rect_T <T>(position + op, size);
		}

		/// This operator creates a rect which completely contains both rects.
		///
		/// @param [in]	rect	the rect to be included in the first rect.
		///
		/// @return		A new rectangle object which contains both rects.
		Rect_T <T> operator + (const Rect_T <T> & op) const {
			const T leftX = std::min(getLeft(), op.getLeft());
			const T rightX = std::max(getRight(), op.getRight());
			const T topY = std::min(getTop(), op.getTop());
			const T bottomY = std::max(getBottom(), op.getBottom());
			return Rect_T <T>(leftX, topY, rightX - leftX, bottomY - topY);
		}

		/// This operator adds an offset to the rectangle object and return
		/// the result.
		///
		/// @param [in]	op	The negative offset value.
		///
		/// @return		A new rectangle object which has the specified
		/// 				offset to the current one.
		Rect_T <T> operator - (const Vector2D_T <T> & op) const {
			return Rect_T <T>(position - op, size);
		}

		/// This operator adds an offset to the current rectangle instance.
		///
		/// @param [in]	op	The offset value.
		///
		/// @return		A reference to the current rectangle object.
		Rect_T <T> & operator += (const Vector2D_T <T> & op) {
			position += op;
			return *this;
		}

		/// This operator adds an offset to the current rectangle instance.
		///
		/// @param [in]	op	The negative offset value.
		///
		/// @return		A reference to the current rectangle object.
		Rect_T <T> & operator -= (const Vector2D_T <T> & op) {
			position -= op;
			return *this;
		}

		template <typename S>
		friend std::wostream& operator<<(std::wostream& out, const Rect_T<S>& rect);
	};

	template <typename S>
	std::wostream& operator<< (std::wostream& out, const Rect_T <S>& rect)  {
		out << "Rect_T (" << rect.position.x << ", " << rect.position.y << ", "
			<< rect.size.width << ", " << rect.size.height << ")";
		return out;
	}

	typedef Rect_T <int32_t> Rect32I;
	typedef Rect_T <int64_t> Rect64I;

	typedef Rect_T <float> Rect32F;
	typedef Rect_T <double> Rect64F;
	typedef Rect_T <Real> RectR;

	typedef Rect_T <Real> Rect;
}

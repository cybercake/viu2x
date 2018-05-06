/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Config.h"
#include "Exceptions.h"
#include "String.h"

namespace v2x {

	/// Class for matrix, vector and their calculations.
	///
	/// 2D, 3D vectors and 2D Size are consider as aliases of matrix with certain 
	/// sizes.
	///
	/// In general its elements can be accessed (r/w) like an 2D array e.g. 
	/// m[row][col]; This class also provides some member functions specific to a 
	/// cerntain size:
	/// -	For 2 x 1 matrix: x(), y(), dot(), width(), height()
	/// -	For 3 x 1 matrix: x(), y(), z(), dot(), cross()
	///
	/// It supports the operator "=" for assigments between matrices.
	/// It supports the operator "*=", "/=" for scaling and "*" for matrix multiplication.
	/// It supports the operator "+" and "-" for element-wise addition and subtraction
	/// It supports the operator "==" and "!=" for comparison.
	///
	/// It has an elimination method which can be used by matrix-inversion.
	///
	/// The parameter type T can be any signed types which support algebra calculations,
	/// typically "int32_t", "int64_t", "float" and "double".
	///
	/// The parameter ROWS and COLS specify the size of the matrix.
	/// 
	/// Content of the matrix will be set to zero on construction by default.
	template <typename T, size_t ROWS, size_t COLS>
	class Matrix_T final {
	public:

		using Row = T[COLS];

	private:

		/// An 2D array for the matrix storage.
		/// m_elements[j] returns the j-th row.
		/// m_elements[j][j] returns the i-th element in the j-th row.
		Row m_elements[ROWS];

		/// Exchange the element values between two rows.
		///
		/// @param [in]	row1	The index of the first row.
		/// @param [in]	row2	The index of the second row.
		void swapRow(int row1, int row2) {
			T temp;

			// Do it for all columns...
			for (int i = 0; i < COLS; i++) {

				// Exchange the values in the column.
				temp = m_elements[row1][i];
				m_elements[row1][i] = m_elements[row2][i];
				m_elements[row2][i] = temp;
			}
		}

		/// Exchange the element values between two columns.
		///
		/// @param [in]	col1	The index of the first column.
		/// @param [in]	col2	The index of the second column.
		void swapCol(int col1, int col2) {
			T temp;

			// Do it for all rows...
			for (int i = 0; i < ROWS; i++) {

				// Exchange the values in the row.
				temp = m_elements[i][col1];
				m_elements[i][col1] = m_elements[i][col2];
				m_elements[i][col2] = temp;
			}
		}

		/// Find the first non-zero cell in the specified column below the specified row.
		///
		/// @param [in]	fromRow		The start position of the search.
		/// @param [in]	col 		The column in which the non-zero value should be found.
		///
		/// @return A value >= 0 if found or -1 if not found.
		int findNonZeroElementInCol(int fromRow, int col) {
			// Do it for all rows in the specified range...
			for (int i = fromRow; i < ROWS; i++)
				// Check if element value is not zero
				if (m_elements[i][col] != 0)
					// If true, return the current row index.
					return i;

			// Return -1 if all elements are zero.
			return -1;
		}

		/// Find the first non-zero cell in the specified row after the specified column.
		///
		/// @param [in] 	fromCol 	The start position of the search.
		/// @param [in] 	row 		The row in which the non-zero value should be found.
		///
		/// @return A value >= 0 if found or -1 if not found.
		int findNonZeroElementInRow(int row, int fromCol) {
			// Do it for all columns in the specified range.
			for (int i = fromCol; i < COLS; i++)
				// Check if the element value is not zero.
				if (m_elements[row][i] != 0)
					// If true, return the current column index.
					return i;

			// Retrun -1 if all elements are zero.
			return -1;
		}

		/// Multiply srcRow with the specified factor and add it to dstRow.
		/// dstRowElement = dstRowElement + factor * srcRowElement
		///
		/// @param [in]	dstRow	The row whose elements should be changed.
		/// @param [in]	srcRow	The row whose elements should be scaled and added to the dstRow.
		/// @param [in]	factor	The factor for the multiplication.
		void addRowWithFactor(int dstRow, int srcRow, T factor) {
			// Do it for all columns...
			for (int i = 0; i < COLS; i++)
				// multiply and add.
				m_elements[dstRow][i] += m_elements[srcRow][i] * factor;
		}

		/// Scale the elements in the specified row with the specified factor.
		///
		/// @param [in]	row		The index of the row to be scaled.
		/// @param [in]	factor	The scaling factor.
		void scaleRow(int row, T factor) {
			// Do it for all columns...
			for (int i = 0; i < COLS; i++)
				// Scale the elements in the specified row.
				m_elements[row][i] *= factor;
		}

	public:

		/// The default constructor. All elements are initialized with 0.
		Matrix_T() {

			// Initialize with zeros
			fill(0);
		}

		/// The constructor initializing all elements using the specified value
		Matrix_T(const T & value) {

			// Initialize with zeros
			fill(value);
		}

		/// The constructor copying data from another matrix.
		///
		/// @param [in]	matrix	An existing matrix from which the data should be copied.
		template<typename OTHER_TYPE>
		explicit Matrix_T(Matrix_T <OTHER_TYPE, ROWS, COLS> const & matrix) {

			// Copy all elements from the source.
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					m_elements[j][i] = static_cast<T>(matrix[j][i]);
		}

		/// The constructor for a 2D vector (2x1 matrix).
		/// Its available ONLY for 2x1 matrices which cold be used as a 2D size
		template<typename TX, typename TY,
			typename TEST = std::enable_if<ROWS == 2 && COLS == 1 && 
			std::is_convertible<TX, T>::value && 
			std::is_convertible<TY, T>::value, T>::type>
		Matrix_T(const TX & x, const TY & y) {
			m_elements[0][0] = x;
			m_elements[1][0] = y;
		}

		/// The constructor for a 2D vector (2x1 matrix).
		/// Its available ONLY for 3x1 matrices which cold be used as a 2D size
		template<typename TX, typename TY, typename TZ,
			typename TEST = std::enable_if<ROWS == 3 && COLS == 1 &&
			std::is_convertible<TX, T>::value &&
			std::is_convertible<TY, T>::value &&
			std::is_convertible<TZ, T>::value, T>::type>
		Matrix_T(const TX & x, const TY & y, const TZ & z) {
			m_elements[0][0] = x;
			m_elements[1][0] = y;
			m_elements[2][0] = z;
		}

		/// The destructor.
		~Matrix_T() {
		}

		/// Fill the whole matrix with the specified value.
		///
		/// @param [in]	value	the value to be filled.
		void fill(const T & value) {
			
			// Apply the value to all elements.
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					m_elements[j][i] = value;
		}

		/// @return The number of columns
		int getColCount() const {
			return COLS;
		}

		/// @return The number of rows
		int getRowCount() const {
			return ROWS;
		}

		/// Operator overloaded for array-like access. You can read/write the elements like:
		/// matrix[row][col] = xxx; or xxx = matrix[row][col];
		///
		/// Ownership NOT transfered. But you can change the content of the returned array pointer.
		///
		/// Note: due to consideration for performance, the storage pointer is not checked in this method!
		///
		/// @param [in]	row		The row index.
		///
		/// @return The pointer to the required row. This pointer can be used like an array.
		///
		/// @todo (RT) Would be nice that to send me a email telling if m_elements should be checked?
		Row & operator[] (int row) {

			// Check the index range.
			if (row < 0 || row >= ROWS)
				throw Exception(L"Matrix_T::[]: index out of range!");

			return m_elements[row];
		}

		/// Operator overloaded for array-like access. You can read/write the elements like:
		/// matrix[row][col] = xxx; or xxx = matrix[row][col];
		///
		/// Ownership NOT transfered. But you can change the content of the returned array pointer.
		///
		/// Note: due to consideration for performance, the storage pointer is not checked in this method!
		///
		/// @param [in]	row		The row index.
		///
		/// @return The pointer to the required row. This pointer can be used like an array.
		///
		/// @todo (RT) Would be nice that to send me a email telling if m_elements should be checked?
		const Row & operator[] (int row) const {

			// Check the index range.
			if (row < 0 || row >= ROWS)
				throw Exception(L"Matrix_T::[]: index out of range!");

			return m_elements[row];
		}

		/// operator overloaded for comparison of equality.
		///
		/// @param [in]	op	The matrix to compare to.
		template <typename OTHER_TYPE>
		bool operator == (const Matrix_T <OTHER_TYPE, ROWS, COLS> & op) const {

			// compare all elements
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++) {
					if (m_elements[j][i] != op[j][i])
						return false;
				}

			return true;
		}

		/// operator overloaded for comparison of equality.
		///
		/// @param [in]	op	The matrix to compare to.
		template <typename OTHER_TYPE>
		bool operator != (const Matrix_T <OTHER_TYPE, ROWS, COLS> & op) const {

			// compare all elements
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++) {
					if (m_elements[j][i] != op[j][i])
						return true;
				}

			return false;
		}

		/// operator overloaded for assignment.
		///
		/// @param [in]	op	The matrix from which the data should be copied.
		template<typename OTHER_TYPE, typename TEST = 
			std::enable_if<std::is_convertible<OTHER_TYPE, T>, T>::type>
		Matrix_T <T, ROWS, COLS> & operator = (const Matrix_T <OTHER_TYPE, ROWS, COLS> & op) {

			// Copy all elements
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					m_elements[j][i] = static_cast<T>(op[j][i]);

			return *this;
		}

		/// Operator overloaded for adding two matrices with the same size
		/// The return type is the type of T + OTHER_TYPE
		template <typename OTHER_TYPE>
		auto operator + (
			const Matrix_T <OTHER_TYPE, ROWS, COLS> & op) const
			-> Matrix_T <decltype(m_elements[0][0] + op[0][0]), ROWS, COLS> {

			// Define the result and give the matrix size.
			Matrix_T <decltype(m_elements[0][0] + op[0][0]), ROWS, COLS> result;

			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					result[row][col] = m_elements[row][col] + op[row][col];

			return result;
		}

		/// Operator overloaded for subtracting two matrices with the same size
		/// The return type is the type of T - OTHER_TYPE
		template <typename OTHER_TYPE>
		auto operator - (
			const Matrix_T <OTHER_TYPE, ROWS, COLS> & op) const
			-> Matrix_T <decltype(m_elements[0][0] - op[0][0]), ROWS, COLS> {

			// Define the result and give the matrix size.
			Matrix_T <decltype(m_elements[0][0] - op[0][0]), ROWS, COLS> result;

			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					result[row][col] = m_elements[row][col] + op[row][col];

			return result;
		}

		/// Operator overloaded for multiplying a matrix with a scalar
		/// The return type is the type of T * OTHER_TYPE
		template <typename OTHER_TYPE>
		auto operator * (const OTHER_TYPE & op) const
			-> Matrix_T <decltype(m_elements[0][0] * op), ROWS, COLS> {

			// Define the result and give the matrix size.
			Matrix_T <decltype(m_elements[0][0] * op), ROWS, COLS> result;

			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					result[row][col] = m_elements[row][col] * op;

			return result;
		}

		/// Operator overloaded for dividing a matrix by a scalar
		/// The return type is the type of T / OTHER_TYPE
		template <typename OTHER_TYPE>
		auto operator / (const OTHER_TYPE & op) const
			-> Matrix_T <decltype(m_elements[0][0] / op), ROWS, COLS> {

			// Check the operand.
			if (op == 0)
				throw Exception(L"Matrix_T::/: The input operand is zero!");

			// Define the result and give the matrix size.
			Matrix_T <decltype(m_elements[0][0] / op), ROWS, COLS> result;

			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					result[row][col] = m_elements[row][col] / op;

			return result;
		}

		/// Operator overloaded for multiplication.
		/// The return type is the type of T * OTHER_TYPE
		template <typename OTHER_TYPE, size_t OTHER_COLS>
		auto operator * (
			const Matrix_T <OTHER_TYPE, COLS, OTHER_COLS> & op) const 
			-> Matrix_T <decltype(m_elements[0][0] * op[0][0]), ROWS, OTHER_COLS> {

			// Define the result and give the matrix size.
			Matrix_T <decltype(m_elements[0][0] * op[0][0]), ROWS, OTHER_COLS> result;

			// Perform the multiplication using a unoptimized method.
			// Note: for 3x3 and 4x4 matrices, a faster multiplication algorithm is used.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < OTHER_COLS; col++) {
					T value = 0;
					for (int i = 0; i < COLS; i++)
						value += static_cast<T>(m_elements[row][i] * op[i][col]);
					result[row][col] = value;
				}

			return result;
		}

		/// Operator overloaded for adding a matrix to the current instance
		template <typename OTHER_TYPE>
		Matrix_T <T, ROWS, COLS> & operator += (
			const Matrix_T <OTHER_TYPE, ROWS, COLS> & op) {
			
			// Scale all elements.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					m_elements[row][col] = static_cast<T>(op[row][col] + m_elements[row][col]);

			return *this;
		}

		/// Operator overloaded for subtracting a matrix from the current instance
		template <typename OTHER_TYPE>
		Matrix_T <T, ROWS, COLS> & operator -= (
			const Matrix_T <OTHER_TYPE, ROWS, COLS> & op) {

			// Scale all elements.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					m_elements[row][col] = static_cast<T>(op[row][col] - m_elements[row][col]);

			return *this;
		}

		/// Operator overloaded for scalar multiplication.
		template <typename OTHER_TYPE>
		Matrix_T <T, ROWS, COLS> & operator *= (const OTHER_TYPE & op) {

			// Scale all elements.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					m_elements[row][col] = static_cast<T>(m_elements[row][col] * op);

			return *this;
		}

		/// Operator overloaded for scalar division.
		///
		/// @throw Exception if the input operand is zero.
		template <typename OTHER_TYPE>
		Matrix_T <T, ROWS, COLS> & operator /= (const OTHER_TYPE & op) {
			
			// Check the operand.
			if (op == 0)
				throw Exception(L"Matrix_T::/=: The input operand is zero!");

			// Scale all elements.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					m_elements[row][col] /= op;

			return *this;
		}

		/// Dot product of two vectors
		template <typename OTHER_TYPE, 
			typename TEST = std::enable_if<(ROWS == 2 || ROWS == 3) && COLS == 1, T>::type>
		auto dot (const Matrix_T<OTHER_TYPE, ROWS, COLS> & op) const 
			-> decltype(m_elements[0][0] * op[0][0]) {

			decltype(m_elements[0][0] * op) result = 0;

			// Scale all elements.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					result += m_elements[row][col] * op[row][col];

			return result;
		}

		/// Cross product of two vectors
		template <typename OTHER_TYPE,
			typename TEST = std::enable_if<ROWS == 3 && COLS == 1, T>::type>
			auto cross(const Matrix_T<OTHER_TYPE, 3, 1> & op) const
			->Matrix_T<decltype(m_elements[0][0] * op[0][0]), 3, 1> {

			Matrix_T<decltype(m_elements[0][0] * op[0][0]), ROWS, COLS> result;
			result[0][0] = m_elements[1][0] * op[2][0] - m_elements[2][0] * op[1][0];
			result[1][0] = m_elements[2][0] * op[0][0] - m_elements[0][0] * op[2][0];
			result[2][0] = m_elements[0][0] * op[1][0] - m_elements[1][0] * op[0][0];
			return result;
		}

		/// This function returns the square of the L2 norm of the matrix using the 
		/// specified type. If you call this on an integer matrix and a floating 
		/// point result, you would not lose precision.
		double normSqr() const {

			double sumSqr = 0;

			// Scale all elements.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					sumSqr += m_elements[row][col] * m_elements[row][col];

			return sumSqr;
		}

		/// This function returns the L2 norm of the matrix using the specified type.
		/// If you call this on an integer matrix and a floating point result, you 
		/// would not lose precision.
		double norm() const {

			double sumSqr = 0;

			// Scale all elements.
			for (int row = 0; row < ROWS; row++)
				for (int col = 0; col < COLS; col++)
					sumSqr += m_elements[row][col] * m_elements[row][col];

			return std::sqrt(sumSqr);
		}

		/// Try to eliminate a square sub-matrix of the current matrix object to diagonal matrix.
		/// |        ...        |    |        ...        |
		/// | ... | a b c | ... |    | ... | 1 0 0 | ... |
		/// | ... | d e f | ... | -> | ... | 0 1 0 | ... |
		/// | ... | g h i | ... |    | ... | 0 0 1 | ... |
		/// |        ...        |    |        ...        |
		/// This can be used to find the inverse of a square matrix (The GAUSS-JORDAN method).
		///
		/// @param [in]	fromCol		the left side of the sub-matrix.
		/// @param [in]	fromRow		the top side of the sub-matrix.
		/// @param [in]	toCol		the right side of the sub-matrix.
		/// @param [in]	toRow		the bottom side of the sub-matrix.
		///
		/// @throw Exception if the sub-matrix cannot be eliminated.
		/// @throw Exception if the specified zone is not a square matrix.
		/// @throw Exception if the specified row-/column-index out of range.
		void eliminate(int fromCol, int fromRow, int toCol, int toRow) {

			// Check if the input column-/row-indices is in a valid range.
			if (fromCol < 0 || toCol < fromCol || toCol >= COLS || //
				fromRow < 0 || toRow < fromRow || toRow >= ROWS)
				throw Exception(L"Matrix_T::eliminate: Column or row index out of range!");

			// Calculate the size of the zone.
			int dCol = toCol - fromCol + 1;
			int dRow = toRow - fromRow + 1;

			// Check if the zone is a square matrix
			if (dCol != dRow)
				throw Exception(L"Matrix_T::eliminate: Only square sub-matrix can be eliminated!");

			// Check if the zone has at more than one column/row.
			if (dCol > 1) {

				// First round: eliminate from left to right.
				// After this round, all elements (of the sub-matrix) below the diagonal should be 0,
				// and all elements on the diagonal should be 1.

				// d is the offset of the column and row index.
				int d = 0;
				while (d < dCol) {

					// Compute the column/row index of the current element on the diagonal.
					int current_col = fromCol + d;
					int current_row = fromRow + d;

					// Check if the current diagonal element is not yet 1. (If it equals 1, nothing has to be done)
					if (m_elements[current_row][current_col] != 1) {

						// Check if the element is 0. If true, it has to be adapted to non-zero with the help of another row.
						if (m_elements[current_row][current_col] == 0) {

							// Find a row below (in the current column) at which the element is not 0.
							// Note: why BELOW? Because the elements on the left to the current diagonal element have to be kept 0,
							// and only the elements below the current row are 0s and they have no effect to the elements which
							// should keep 0.
							int nz = findNonZeroElementInCol(current_row + 1, current_col);

							// Check if the row is found.
							if (nz < 0)
								// If not found, it means the matrix cannot be eliminated.
								throw Exception(L"Matrix_T::eliminate: The specified zone cannot be eliminated!");

							// Adapt the current row.
							// After this operation, the current diagonal element should be 1 and all elements (in the sub-matrix and
							// in the same row) on the left to it should be 0.
							addRowWithFactor(current_row, nz, 1 / m_elements[nz][current_col]);
						}
						else {
							// If the current diagonal element is not 0, then simply scale it to 1.
							scaleRow(current_row, 1 / m_elements[current_row][current_col]);
						}
					}

					// Do it for all rows in the sub-martirx and below the current row...
					for (int r = current_row + 1; r <= toRow; r++)
						// Check if the element is zero. If it is zero, nothing has to be done.
						if (m_elements[r][current_col] != 0)
							// Make it zero by subtract a scaled version of the current_row.
							addRowWithFactor(r, current_row, -m_elements[r][current_col]);

					// Increase the offset
					d++;
				}

				// Second round: eliminate from right to left.
				// After this round, all elements above the diagonal should also be 0.

				// d is the offset of the column and row index.
				d = dCol - 1;
				while (d > 0) {

					// Compute the column/row index of the current element on the diagonal.
					int current_col = fromCol + d;
					int current_row = fromRow + d;

					// Check if the current diagonal element is 0. (After the first round, this should not
					// be true. Just left here for higher security)
					if (m_elements[current_row][current_col] == 0)
						// If true, this matrix cannot be eliminated.
						throw Exception(L"Matrix_T::eliminate: The specified zone cannot be eliminated!");

					// Do it for all rows (in the same column) above the diagonal element.
					for (int r = current_row - 1; r >= fromRow; r--) {

						// Check if the element is 0. If it is zero, nothing has to be done.
						if (m_elements[r][current_col] != 0)
							// Make the element zero by subtracting a scaled version of the current_row.
							addRowWithFactor(r, current_row, -m_elements[r][current_col]);
					}

					// Decrease the offset.
					d--;
				}
			}
			else {
				// Check if the element is zero.
				if (m_elements[fromRow][fromCol] == 0)
					// if true, the matrix cannot be eliminated.
					throw Exception(L"Matrix_T::eliminate: The specified zone cannot be eliminated!");

				// To avoid divisions, just compute a scale factor first, then only multiplications are used later.
				T factor = 1 / m_elements[fromRow][fromCol];

				// Scale all elements using multiplication instead of division.
				for (int i = 0; i < COLS; i++)
					m_elements[fromRow][i] *= factor;
			}
		}

		/// Returns a transposed version of the current matrix. The current matrix is not changed.
		///
		/// For example:
		/// | a b c ... |    | a d ... |
		/// | d e f ... | -> | b e ... |
		/// |    ...    |    | c f ... |
		/// |    ...    |    |   ...   |
		Matrix_T <T, COLS, ROWS> transpose() const {

			// Define the result.
			Matrix_T <T, COLS, ROWS> result;

			// Copy all elements.
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					result[i][j] = m_elements[j][i];

			return result;
		}

		/// This function copies elements from the specified area of the
		/// source matrix.
		///
		/// @param [in]	source			The matrix from which the elements
		/// 								will be copied.
		/// @param [in]	fromSrcRow		The starting row of copying.
		/// @param [in]	fromSrcCol		The starting column of copying.
		/// @param [in]	toDstRow		The starting target row.
		/// @param [in]	toDstCol		The starting target column.
		/// @param [in]	numberOfRows	The number of row to be copied.
		/// @param [in]	numberOfCols	The number of columns to be copied.
		///
		/// @throw		Exception if any of the indices is out of range.
		template <typename OTHER_TYPE, size_t OTHER_ROWS, size_t OTHER_COLS,
			typename TEST = std::enable_if<std::is_convertible<T, OTHER_TYPE>, T>::type>
		void copyFrom(const Matrix_T <OTHER_TYPE, OTHER_ROWS, OTHER_COLS> source, //
			int fromSrcRow, int fromSrcCol, //
			int toDstRow, int toDstCol, //
			int numberOfRows, int numberOfCols) {

			if (fromSrcRow < 0 || (fromSrcRow + numberOfRows) > OTHER_ROWS ||
				fromSrcCol < 0 || (fromSrcCol + numberOfCols) > OTHER_COLS ||
				toDstRow < 0 || (toDstRow + numberOfRows) > ROWS || 
				toDstCol < 0 || (toDstCol + numberOfCols) > COLS || ) {
				throw Exception(L"Matrix_T::copyFrom: Column or row index out of range!");
			}

			int dst_r = toDstRow;
			for (int src_r = fromSrcRow; src_r < fromSrcRow + numberOfRows; src_r++) {
				int dst_c = toDstCol;
				for (int src_c = fromSrcCol; src_c < fromSrcCol + numberOfCols; src_c++) {
					m_elements[dst_r][dst_c] = source[src_r][src_c];
					dst_c++;
				}
				dst_r++;
			}
		}

		/// This function copies a sub-block of the current matrix to a new matrix.
		template <typename OTHER_TYPE, size_t OTHER_ROWS, size_t OTHER_COLS,
			typename TEST = std::enable_if<std::is_convertible<T, OTHER_TYPE>, T>::type>
		Matrix_T <OTHER_TYPE, OTHER_ROWS, OTHER_COLS> subMatrix(int fromRow, int fromCol) {

			if (fromRow < 0 || (fromRow + OTHER_ROWS) >  ROWS ||
				fromCol < 0 || (fromCol + OTHER_COLS) >  COLS) {
				throw Exception(L"Matrix_T::copyFrom: Column or row index out of range!");
			}

			Matrix_T <OTHER_TYPE, OTHER_ROWS, OTHER_COLS> result;

			int dst_r = 0;
			for (int src_r = fromRow; src_r < fromRow + OTHER_ROWS; src_r++) {
				int dst_c = 0;
				for (int src_c = fromCol; src_c < fromCol + OTHER_COLS; src_c++) {
					result[dst_r][dst_c] = m_elements[src_r][src_c];
					dst_c++;
				}
				dst_r++;
			}

			return result;
		}

		/// Returns a string representation of the matrix in C++ syntax.
		String toString() const {
			std::wostringstream ss;
			ss.precision(32);
			ss << "{";
			for (int i = 0; i < ROWS; i++) {
				ss << "{";
				for (int j = 0; j < COLS; j++) {
					ss << m_elements[i][j];
					if (j < COLS - 1)
						ss << ", ";
				}
				ss << "}";
				if (j < ROWS - 1)
					ss << ", ";
			}
			ss << "}";

			return ss.str();
		}

		/// Return true if all elements are normal numbers.
		bool isNormal() const {

			// Check all elements
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					if (!std::isnormal(m_elements[j][i])) return false;

			return true;
		}

		/// Return true if at least one element is INFINITY
		bool isInf() const {
		
			// Check all elements
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					if (std::isinf(m_elements[j][i])) return true;

			return false;
		}

		/// Return true if at least one element is NAN
		bool isNaN() const {

			// Check all elements
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					if (std::isnan(m_elements[j][i])) return true;

			return false;
		}

		/// Return true if all elements are zero
		bool isZero() const {

			// Check all elements
			for (int j = 0; j < ROWS; j++)
				for (int i = 0; i < COLS; i++)
					if (!(m_elements[j][i] == 0)) return false;

			return true;
		}

		/// Returns a readonly reference to the ELEMENT[0][0]
		/// Its available ONLY for 2x1 or 3x1 matrices which cold be used as a 2D vector
		template<typename T2 = std::enable_if<(ROWS == 2 || ROWS == 3) && COLS == 1, T>::type>
		const T2 & x() const { return m_elements[0][0]; }

		/// Returns a writebale reference to the ELEMENT[0][0]
		/// Its available ONLY for 2x1 or 3x1 matrices which cold be used as a 2D vector
		template<typename T2 = std::enable_if<(ROWS == 2 || ROWS == 3) && COLS == 1, T>::type>
		T2 & x() { return m_elements[0][0]; }

		/// Returns a readonly reference to the ELEMENT[1][0]
		/// Its available ONLY for 2x1 or 3x1 matrices which cold be used as a 2D vector
		template<typename T2 = std::enable_if<(ROWS == 2 || ROWS == 3) && COLS == 1, T>::type>
		const T2 & y() const { return m_elements[1][0]; }

		/// Returns a writebale reference to the ELEMENT[1][0]
		/// Its available ONLY for 2x1 or 3x1 matrices which cold be used as a 2D vector
		template<typename T2 = std::enable_if<(ROWS == 2 || ROWS == 3) && COLS == 1, T>::type>
		T2 & y() { return m_elements[1][0]; }

		/// Returns a readonly reference to the ELEMENT[2][0]
		/// Its available ONLY for 3x1 matrices which cold be used as a 2D vector
		template<typename T2 = std::enable_if<ROWS == 3 && COLS == 1, T>::type>
		const T2 & z() const { return m_elements[2][0]; }

		/// Returns a writebale reference to the ELEMENT[3][0]
		/// Its available ONLY for 3x1 matrices which cold be used as a 2D vector
		template<typename T2 = std::enable_if<ROWS == 3 && COLS == 1, T>::type>
		T2 & z() { return m_elements[2][0]; }
		
		/// Returns a readonly reference to the ELEMENT[0][0]
		/// Its available ONLY for 2x1 matrices which cold be used as a 2D size
		template<typename T2 = std::enable_if<ROWS == 2 && COLS == 1, T>::type>
		const T2 & width() const { return m_elements[0][0]; }

		/// Returns a writeable reference to the ELEMENT[0][0]
		/// Its available ONLY for 2x1 matrices which cold be used as a 2D size
		template<typename T2 = std::enable_if<ROWS == 2 && COLS == 1, T>::type>
		T2 & width() { return m_elements[0][0]; }

		/// Returns a readonly reference to the ELEMENT[1][0]
		/// Its available ONLY for 2x1 matrices which cold be used as a 2D size
		template<typename T2 = std::enable_if<ROWS == 2 && COLS == 1, T>::type>
		const T2 & height() const { return m_elements[1][0]; }

		/// Returns a writebale reference to the ELEMENT[1][0]
		/// Its available ONLY for 2x1 matrices which cold be used as a 2D size
		template<typename T2 = std::enable_if<ROWS == 2 && COLS == 1, T>::type>
		T2 & height() { return m_elements[1][0]; }
	};

	template<typename T>
	using Vector2D_T = Matrix_T<T, 2, 1>;
	using Vector2D32I = Vector2D_T<int32_t>;
	using Vector2D64I = Vector2D_T<int64_t>;
	using Vector2D32F = Vector2D_T<float>;
	using Vector2D64F = Vector2D_T<double>;
	using Vector2D = Vector2D_T<double>;
	
	template<typename T>
	using Vector3D_T = Matrix_T<T, 3, 1>;
	using Vector3D32I = Vector3D_T<int32_t>;
	using Vector3D64I = Vector3D_T<int64_t>;
	using Vector3D32F = Vector3D_T<float>;
	using Vector3D64F = Vector3D_T<double>;
	using Vector3D = Vector3D_T<double>;

	template<typename T>
	using Size2D_T = Matrix_T<T, 2, 1>;
	using Size2D32I = Size2D_T<int32_t>;
	using Size2D64I = Size2D_T<int64_t>;
	using Size2D32F = Size2D_T<float>;
	using Size2D64F = Size2D_T<double>;
	using Size2D = Size2D_T<double>;

	template<size_t ROWS, size_t COLS>
	using Matrix32I = Matrix_T<int32_t, ROWS, COLS>;

	template<size_t ROWS, size_t COLS>
	using Matrix64I = Matrix_T<int64_t, ROWS, COLS>;

	template<size_t ROWS, size_t COLS>
	using Matrix64F = Matrix_T<double, ROWS, COLS>;

	template<size_t ROWS, size_t COLS>
	using Matrix32F = Matrix_T<float, ROWS, COLS>;

	template<size_t ROWS, size_t COLS>
	using Matrix = Matrix_T<double, ROWS, COLS>;
}
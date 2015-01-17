/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Config.h"
#include "Exceptions.h"
#include "String.h"

namespace v2x {

	/// Class for matrix calculations.
	///
	/// Its elements can be accessed (r/w) like an 2D array e.g. m1[row][col] = m2[row][col];
	///
	/// It supports the operation "=" for assigments between matrices.
	/// It supports the operation "*=", "/=" for scalar calculations and "*" for matrix multiplication.
	///
	/// It has an elimination method which can be used by matrix-inversion.
	///
	/// The parameter type T can be any signed types which support algebra calculations,
	/// typically "float" and "double".
	template <typename T>
	class Matrix {
	protected:

		/// Number of the columns.
		int m_colCount;

		/// Number of the rows.
		int m_rowCount;

		/// An 2D array for the matrix storage.
		/// m_elements[i] returns the i-th row.
		/// m_elements[i][j] returns the j-th element in the i-th row.
		T** m_elements;

		/// Allocate a memory space to hold the matrix data.
		/// If the internal storage is not NULL (i.e. it has been initialized), nothing will be done.
		/// @throw Exception if the number of columns or rows is zero.
		virtual void initializeStorage() {

			// Check if the storage has been allocated.
			if (m_elements == NULL) {

				// Check if the number of columns and rows are valid.
				if (m_colCount <= 0 || m_rowCount <= 0)
					throw Exception(
					L"Matrix::initializeStorage: The number of rows and columns should not be zero!");

				// Allocates spaces for all row pointers.
				m_elements = (T**)malloc(m_rowCount * sizeof(T*));
				// For all rows...
				for (int i = 0; i < m_rowCount; i++)
					// Allocate space for each row.
					m_elements[i] = (T*)malloc(m_colCount * sizeof(T));
			}
		}

		/// Release an allocated matrix storage and set it to NULL.
		///
		/// If the internal storage is NULL (i.e. it has been released), nothing will be done.
		virtual void finalizeStorage() {

			// Check if the storage has been allocated.
			if (m_elements != NULL) {

				// For all rows...
				for (int i = 0; i < m_rowCount; i++)
					// Rrelease the row data.
					free(m_elements[i]);
				// Release the pointer to rows.
				free(m_elements);

				// Reset storage.
				m_elements = NULL;
			}
		}

		/// Exchange the element values between two rows.
		///
		/// @param [in]	row1	The index of the first row.
		/// @param [in]	row2	The index of the second row.
		virtual void swapRow(int row1, int row2) {
			T temp;

			// Do it for all columns...
			for (int i = 0; i < m_colCount; i++) {

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
		virtual void swapCol(int col1, int col2) {
			T temp;

			// Do it for all rows...
			for (int i = 0; i < m_rowCount; i++) {

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
		virtual int findNonZeroElementInCol(int fromRow, int col) {
			// Do it for all rows in the specified range...
			for (int i = fromRow; i < m_rowCount; i++)
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
		virtual int findNonZeroElementInRow(int row, int fromCol) {
			// Do it for all columns in the specified range.
			for (int i = fromCol; i < m_colCount; i++)
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
		virtual void addRowWithFactor(int dstRow, int srcRow, T factor) {
			// Do it for all columns...
			for (int i = 0; i < m_colCount; i++)
				// multiply and add.
				m_elements[dstRow][i] += m_elements[srcRow][i] * factor;
		}

		/// Scale the elements in the specified row with the specified factor.
		///
		/// @param [in]	row		The index of the row to be scaled.
		/// @param [in]	factor	The scaling factor.
		virtual void scaleRow(int row, T factor) {
			// Do it for all columns...
			for (int i = 0; i < m_colCount; i++)
				// Scale the elements in the specified row.
				m_elements[row][i] *= factor;
		}

	public:

		/// The default constructor.
		Matrix() {
			m_colCount = 0;
			m_rowCount = 0;
			m_elements = NULL;
		}

		/// The constructor enabling building a matrix with the specified sizes.
		/// The value of its elements are UNDEFINED after construction.
		///
		/// @param [in]	rows	The number of rows. It has to be an integer greater than 0.
		/// @param [in]	cols	The number of columns. It has to be an integer greater than 0.
		///
		/// @throw @see initializeStorage().
		Matrix(int rows, int cols) {
			m_colCount = cols;
			m_rowCount = rows;
			m_elements = NULL;

			// Allocate memory.
			initializeStorage();
		}

		/// The constructor copying data from another matrix.
		///
		/// @param [in]	matrix	An existing matrix from which the data should be copied.
		///
		/// @throw @see initializeStorage().
		Matrix(Matrix <T> const & matrix) {
			m_colCount = matrix.getColCount();
			m_rowCount = matrix.getRowCount();
			m_elements = NULL;

			// Allocate memory.
			initializeStorage();

			// Copy all elements from the source.
			for (int j = 0; j < m_rowCount; j++)
				for (int i = 0; i < m_colCount; i++)
					m_elements[j][i] = matrix[j][i];
		}

		/// The destructor.
		virtual ~Matrix() {
			finalizeStorage();
		}


		/// Fill the whole matrix with the specified value.
		///
		/// @param [in]	value	the value to be filled.
		///
		/// @throw Exception if the internal storage is not initialized.
		virtual void fill(T value) {
			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::fill: The matrix has not been initialized.");

			// Apply the value to all elements.
			for (int j = 0; j < m_rowCount; j++)
				for (int i = 0; i < m_colCount; i++)
					m_elements[j][i] = value;
		}

		/// @return The number of columns
		virtual int getColCount() const {
			return m_colCount;
		}

		/// @return The number of rows
		virtual int getRowCount() const {
			return m_rowCount;
		}

		/// Returns an element of the matrix with storage and range check.
		///
		/// @param [in]	row		The row index.
		/// @param [in]	col		The column index.
		///
		/// @return An element on the specified location.
		///
		/// @throw Exception if the internal storage is not initialized.
		/// @throw Exception if any of the given indices is out of range.
		virtual const T & getElement(int row, int col) const {
			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::getElement: The matrix has not been initialized.");

			// Check the index range.
			if (row < 0 || row >= m_rowCount || col < 0 || col >= m_colCount)
				throw Exception(L"Matrix::getElement: Matrix index out of range!");

			return m_elements[row][col];
		}

		/// Sets the value of a specified element with storage and range check.
		///
		/// @param [in]	row		The row index.
		/// @param [in]	col		The column index.
		/// @param [in]	value	The value to be set.
		///
		/// @throw Exception if the internal storage is not initialized.
		/// @throw Exception if any of the given indices is out of range.
		virtual void setElement(int row, int col, const T & value) {
			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::setElement: The matrix has not been initialized.");

			// Check the index range.
			if (row < 0 || row >= m_rowCount || col < 0 || col >= m_colCount)
				throw Exception(L"Matrix::setElement: Matrix index out of range!");

			m_elements[row][col] = value;
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
		T * operator[] (int row) const {
			return m_elements[row];
		}

		/// operator overloaded for comparison of equality.
		///
		/// @param [in]	op	The matrix to compare to.
		bool operator == (const Matrix <T> & op) const {

			if (m_rowCount != op.m_rowCount) {
				return false;
			}

			if (m_colCount != op.m_colCount) {
				return false;
			}

			// compare all elements
			for (int j = 0; j < m_rowCount; j++)
				for (int i = 0; i < m_colCount; i++)
					if (m_elements[j][i] != op[j][i]) {
				return false;
					}

			return true;
		}

		/// operator overloaded for assignment.
		///
		/// @param [in]	op	The matrix from which the data should be copied.
		///
		/// @throw @see initializeStorage().
		Matrix <T> & operator = (const Matrix <T> & op) {

			// Clear the old data.
			finalizeStorage();

			// Reinitialize the matrix
			m_colCount = op.getColCount();
			m_rowCount = op.getRowCount();
			initializeStorage();

			// Copy all elements
			for (int j = 0; j < m_rowCount; j++)
				for (int i = 0; i < m_colCount; i++)
					m_elements[j][i] = op[j][i];

			return *this;
		}

		/// Operator overloaded for multiplication.
		///
		/// @throw Exception if the internal storage is not initialized.
		/// @throw Exception if the two matrices cannot be multiplied.
		Matrix <T> operator * (const Matrix <T> & op) const {

			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::*: The matrix has not been initialized.");

			// Check if the two matrices can be multiplied!
			if (m_colCount == op.getRowCount()) {

				// Define the result and give the matrix size.
				Matrix <T> result(m_rowCount, op.getColCount());

				// Perform the multiplication using a unoptimized method.
				// Note: for 3x3 and 4x4 matrices, a faster multiplication algorithm is used.
				for (int row = 0; row < result.getRowCount(); row++)
					for (int col = 0; col < result.getColCount(); col++) {
					T value = 0;
					for (int i = 0; i < m_colCount; i++)
						value += m_elements[row][i] * op[i][col];
					result[row][col] = value;
					}
				return result;
			}
			else
				throw Exception(L"Matrix::*: The size of matrices doesn't match in a multiplication!");
		}

		/// Operator overloaded for scalar multiplication.
		///
		/// @throw Exception if the internal storage is not initialized.
		Matrix <T> & operator *= (const T & op) {
			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::*=: The matrix has not been initialized.");

			// Scale all elements.
			for (int row = 0; row < m_rowCount; row++)
				for (int col = 0; col < m_colCount; col++)
					m_elements[row][col] *= op;

			return *this;
		}

		/// Operator overloaded for scalar division.
		///
		/// @throw Exception if the internal storage is not initialized.
		/// @throw Exception if the input operand is zero.
		Matrix <T> & operator /= (const T & op) {
			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::/=: The matrix has not been initialized.");

			// Check the operand.
			if (op == 0)
				throw Exception(L"Matrix::/=: The input operand is zero!");

			// Scale all elements.
			for (int row = 0; row < m_rowCount; row++)
				for (int col = 0; col < m_colCount; col++)
					m_elements[row][col] /= op;

			return *this;
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
		/// @throw Exception if the internal storage is not initialized.
		/// @throw Exception if the sub-matrix cannot be eliminated.
		/// @throw Exception if the specified zone is not a square matrix.
		/// @throw Exception if the specified row-/column-index out of range.
		virtual void eliminate(int fromCol, int fromRow, int toCol, int toRow) {
			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::fill: The matrix has not been initialized.");

			// Check if the input column-/row-indices is in a valid range.
			if (fromCol < 0 || toCol < fromCol || toCol >= m_colCount || //
				fromRow < 0 || toRow < fromRow || toRow >= m_rowCount)
				throw Exception(L"Matrix::eliminate: Column or row index out of range!");

			// Calculate the size of the zone.
			int dCol = toCol - fromCol + 1;
			int dRow = toRow - fromRow + 1;

			// Check if the zone is a square matrix
			if (dCol != dRow)
				throw Exception(L"Matrix::eliminate: Only square sub-matrix can be eliminated!");

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
								throw Exception(L"Matrix::eliminate: The specified zone cannot be eliminated!");

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
						throw Exception(L"Matrix::eliminate: The specified zone cannot be eliminated!");

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
					throw Exception(L"Matrix::eliminate: The specified zone cannot be eliminated!");

				// To avoid divisions, just compute a scale factor first, then only multiplications are used later.
				T factor = 1 / m_elements[fromRow][fromCol];

				// Scale all elements using multiplication instead of division.
				for (int i = 0; i < m_colCount; i++)
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
		///
		/// @throw Exception if the internal storage is not initialized.
		Matrix <T> transpose() const {
			// Check the storage.
			if (m_elements == NULL)
				throw Exception(L"Matrix::fill: The matrix has not been initialized.");

			// Define the result.
			Matrix <T> result(m_colCount, m_rowCount);

			// Copy all elements.
			for (int j = 0; j < m_rowCount; j++)
				for (int i = 0; i < m_colCount; i++)
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
		void copyFrom(const Matrix <T> source, //
			int fromSrcRow, int fromSrcCol, //
			int toDstRow, int toDstCol, //
			int numberOfRows, int numberOfCols) {

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

		/// Returns a string representation of the matrix.
		String toString() const {
			std::wostringstream ss;
			ss.precision(32);
			for (int i = 0; i < m_rowCount; i++) {
				ss << "(";
				for (int j = 0; j < m_colCount; j++) {
					ss << m_elements[i][j];
					if (j < m_colCount - 1)
						ss << ", ";
				}
				ss << ")\n";
			}

			return ss.str();
		}
	};

	typedef Matrix <float> Matrix32F;
	typedef Matrix <double> Matrix64F;
	typedef Matrix <Real> MatrixR;
}
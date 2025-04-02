// Its class Matrix, which contains 2 constructors: one creates a matrix based
// on the number of rows and columns; the second creates a list using {}.
// The matrix also returns the number of rows and columns,
// and makes it possible to read or modify elements via [].

#ifndef CLASS_MATRIX_H
#define CLASS_MATRIX_H

#include <vector>

#include "error_handling.h"

using UnsignedType = size_t;

template <typename T>
class Matrix
{
public:

	Matrix();

	Matrix(const UnsignedType& rows, const UnsignedType& columns);

	Matrix(std::initializer_list<std::vector<T>> values);

	const std::vector<T>& operator[] (const UnsignedType& index) const;

	std::vector<T>& operator[] (const UnsignedType& index);

	UnsignedType sizeRows() const;

	UnsignedType sizeColumns() const;

	void eraseRowMatrix(const UnsignedType& index);

	bool empty() const;

private:

	std::vector<std::vector<T>> m_matrix;

};

#endif
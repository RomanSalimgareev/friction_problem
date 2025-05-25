// This is the matrix class, which provides a constructor without parameters,
// a constructor with a row and column assignment(the matrix is filled with
// zero elements), and a constructor that is initialized with a list. 
// The class also has an overloaded operator [], which makes it possible to
// both read and modify the matrix element.You can get the number of rows,
// the number of columns from an object, delete a row, delete a column,
// and check it for emptiness.

#ifndef CLASS_MATRIX_H
#define CLASS_MATRIX_H

#include <MFE_lib/types/types.h>
#include <MFE_lib/export.h>

namespace MFE
{
	template <typename T>
	class MFE_LIB_EXPORT Matrix
	{
	public:

		Matrix();

		Matrix(const UnsignedType& rows, const UnsignedType& columns);

		Matrix(std::initializer_list<std::vector<T>> values);

		const std::vector<T>& operator[] (const UnsignedType& index) const;

		std::vector<T>& operator[] (const UnsignedType& index);

		UnsignedType sizeRows() const;

		UnsignedType sizeColumns() const;

		void eraseRow(const UnsignedType& index);

		void eraseColumn(const UnsignedType& index);

		bool empty() const;

	private:

		std::vector<std::vector<T>> m_matrix;

	};

}
#endif
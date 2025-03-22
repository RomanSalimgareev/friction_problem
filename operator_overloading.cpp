// This file implements functions from the operator_overloading.h file.

#include "operator_overloading.h"

template <typename T>
std::ostream& operator << (const Matrix<T>& matrix,
	std::ostream& out)
{
	const UnsignedType r = matrix.sizeRows();
	const UnsignedType c = matrix.sizeColumns();

	for (UnsignedType i = 0; i < r; ++i)
	{
		for (UnsignedType j = 0; j < c; ++j)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n\n";
	}
	return out;
}

template <typename T>
std::ostream& operator << (const std::vector<T>& vectorOutput,
	std::ostream& out)
{
	const UnsignedType r = vectorOutput.size();

	for (UnsignedType i = 0; i < r; ++i)
	{
		std::cout << vectorOutput[i] << " ";
	}
	return out;
}

///////////////////////////////////////////////////////////

RealVector operator* (const Real& value,
	const RealVector& vec)
{
	RealVector product = vec;
	const UnsignedType s = vec.size();
	for (UnsignedType i = 0; i < s; ++i)
	{
		product[i] = value * vec[i];
	}
	return product;
}

RealVector operator* (const RealVector& vec,
	const Real& value)
{
	UnsignedType s = vec.size();
	RealVector product(s);
	for (UnsignedType i = 0; i < s; ++i)
	{
		product[i] = value * vec[i];
	}
	return product;
}

RealVector& operator+= (RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	const UnsignedType s = vectorFirst.size();
	for (UnsignedType i = 0; i < s; ++i)
	{
		vectorFirst[i] += vectorSecond[i];
	}
	return vectorFirst;
}

RealVector operator+ (const RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	RealVector result = vectorFirst;
	const UnsignedType s = vectorFirst.size();
	for (UnsignedType i = 0; i < s; ++i)
	{
		result[i] += vectorSecond[i];
	}
	return result;
}

RealVector operator- (const RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	RealVector result = vectorFirst;
	const UnsignedType s = vectorFirst.size();
	for (UnsignedType i = 0; i < s; ++i)
	{
		result[i] -= vectorSecond[i];
	}
	return result;
}

RealVector operator* (const RealMatrix& matrix,
	const RealVector& vec)
{
	const UnsignedType r = matrix.sizeRows();
	const UnsignedType c = matrix.sizeColumns();
	std::vector <Real> product(r);

	for (UnsignedType i = 0; i < r; ++i)
	{
		for (UnsignedType j = 0; j < c; ++j)
		{
			product[i] += matrix[i][j] * vec[j];
		}
	}
	return product;
}

RealVector operator* (const RealVector& vec,
	const RealMatrix& matrix)
{
	const UnsignedType r = matrix.sizeRows();
	const UnsignedType c = matrix.sizeColumns();
	std::vector <Real> product(r);

	for (UnsignedType i = 0; i < r; ++i)
	{
		for (UnsignedType j = 0; j < c; ++j)
		{
			product[i] += matrix[i][j] * vec[j];
		}
	}
	return product;
}

Real operator* (const RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	Real product = 0;
	const UnsignedType s = vectorFirst.size();
	for (UnsignedType i = 0; i < s; ++i)
	{
		product += vectorFirst[i] * vectorSecond[i];
	}
	return product;
}

///////////////////////////////////////////////////////////

RealMatrix operator* (const Real& value,
	const RealMatrix& matrix)
{
	const UnsignedType r = matrix.sizeRows();
	const UnsignedType c = matrix.sizeColumns();
	RealMatrix product(r, c);

	for (UnsignedType i = 0; i < r; ++i)
	{
		for (UnsignedType j = 0; j < c; ++j)
		{
			product[i][j] = matrix[i][j] * value;
		}
	}
	return product;
}

RealMatrix operator* (const RealMatrix& matrix,
	const Real& value)
{
	const UnsignedType r = matrix.sizeRows();
	const UnsignedType c = matrix.sizeColumns();
	RealMatrix product(r, c);

	for (UnsignedType i = 0; i < r; ++i)
	{
		for (UnsignedType j = 0; j < c; ++j)
		{
			product[i][j] = matrix[i][j] * value;
		}
	}
	return product;
}

RealMatrix operator+ (const RealMatrix& matrixFirst,
	const RealMatrix& matrixSecond)
{
	RealMatrix result = matrixFirst;
	const UnsignedType r = matrixFirst.sizeRows();
	const UnsignedType c = matrixFirst.sizeColumns();

	for (UnsignedType i = 0; i < r; ++i)
	{
		for (UnsignedType j = 0; j < c; ++j)
		{
			result[i][j] += matrixSecond[i][j];
		}
	}

	return result;
}

RealMatrix operator- (const RealMatrix& matrixFirst,
	const RealMatrix& matrixSecond)
{
	RealMatrix result = matrixFirst;
	const UnsignedType r = matrixFirst.sizeRows();
	const UnsignedType c = matrixFirst.sizeColumns();

	for (UnsignedType i = 0; i < r; ++i)
	{
		for (UnsignedType j = 0; j < c; ++j)
		{
			result[i][j] -= matrixSecond[i][j];
		}
	}

	return result;
}
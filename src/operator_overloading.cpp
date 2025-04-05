// This file implements functions from the operator_overloading.h file.

#include "operator_overloading.h"

std::string messageSizeFirstLess()
{
	return "Vector size first < vector size second. ";
}

std::string messageSizeMatricestNotEqual()
{
	return "The sizes of the matrices are not equal. ";
}

template <typename T>
std::ostream& operator << (const Matrix<T>& matrix,
	std::ostream& out)
{
	const UnsignedType rows = matrix.sizeRows();
	const UnsignedType columns = matrix.sizeColumns();

	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		for (UnsignedType columnNum = 0; columnNum < columns; ++columnNum)
		{
			std::cout << matrix[rowNum][columnNum] << " ";
		}
		std::cout << "\n\n";
	}
	return out;
}

template <typename T>
std::ostream& operator << (const std::vector<T>& vectorOutput,
	std::ostream& out)
{
	const UnsignedType size = vectorOutput.size();

	for (UnsignedType index = 0; index < size; ++index)
	{
		std::cout << vectorOutput[index] << " ";
	}
	return out;
}

///////////////////////////////////////////////////////////

RealVector operator* (const Real& value,
	const RealVector& vec)
{
	RealVector product = vec;
	const UnsignedType size = vec.size();
	for (UnsignedType index = 0; index < size; ++index)
	{
		product[index] = value * vec[index];
	}
	return product;
}

RealVector operator* (const RealVector& vec,
	const Real& value)
{
	const UnsignedType size = vec.size();
	RealVector product(size);
	for (UnsignedType index = 0; index < size; ++index)
	{
		product[index] = value * vec[index];
	}
	return product;
}

RealVector& operator+= (RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	const UnsignedType sizeSecond = vectorSecond.size();
	if (vectorFirst.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		vectorFirst[indexSecond] += vectorSecond[indexSecond];
	}
	return vectorFirst;
}

RealVector& operator-= (RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	const UnsignedType sizeSecond = vectorSecond.size();
	if (vectorFirst.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		vectorFirst[indexSecond] -= vectorSecond[indexSecond];
	}
	return vectorFirst;
}

RealVector operator+ (const RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	RealVector result = vectorFirst;
	const UnsignedType sizeSecond = vectorSecond.size();
	if (vectorFirst.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		result[indexSecond] += vectorSecond[indexSecond];
	}
	return result;
}

RealVector operator- (const RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	RealVector result = vectorFirst;
	const UnsignedType sizeSecond = vectorSecond.size();
	if (vectorFirst.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		result[indexSecond] -= vectorSecond[indexSecond];
	}
	return result;
}

RealVector operator* (const RealMatrix& matrix,
	const RealVector& vec)
{
	const UnsignedType rowsMatrix = matrix.sizeRows();
	const UnsignedType columnsMatrix = matrix.sizeColumns();
	std::vector <Real> product(rowsMatrix);
	if (vec.size() < columnsMatrix)
	{
		std::string msg = "Vector size < סרהדüעû of the matrix";
		ERROR(msg);
	}

	for (UnsignedType rowNum = 0; rowNum < rowsMatrix; ++rowNum)
	{
		for (UnsignedType columnNum = 0; columnNum < columnsMatrix;
			++columnNum)
		{
			product[rowNum] +=
				matrix[rowNum][columnNum] * vec[columnNum];
		}
	}
	return product;
}

RealVector operator* (const RealVector& vec,
	const RealMatrix& matrix)
{
	const UnsignedType rowsMatrix = matrix.sizeRows();
	const UnsignedType columnsMatrix = matrix.sizeColumns();
	std::vector <Real> product(columnsMatrix);
	if (vec.size() < rowsMatrix)
	{
		std::string msg = "Vector size < rows of the matrix";
		ERROR(msg);
	}

	for (UnsignedType rowNum = 0; rowNum < rowsMatrix; ++rowNum)
	{
		for (UnsignedType columnNum = 0; columnNum < columnsMatrix;
			++columnNum)
		{
			product[rowNum] +=
				matrix[rowNum][columnNum] * vec[columnNum];
		}
	}
	return product;
}

Real operator* (const RealVector& vectorFirst,
	const RealVector& vectorSecond)
{
	Real product = 0;
	const UnsignedType sizeSecond = vectorSecond.size();
	if (vectorFirst.size() != sizeSecond)
	{
		std::string msg = "The size of the vectors are not equal. ";
		ERROR(msg);
	}
	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		product += vectorFirst[indexSecond] * vectorSecond[indexSecond];
	}
	return product;
}

///////////////////////////////////////////////////////////

RealMatrix operator* (const Real& value,
	const RealMatrix& matrix)
{
	const UnsignedType rows = matrix.sizeRows();
	const UnsignedType columns = matrix.sizeColumns();
	RealMatrix product(rows, columns);

	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		for (UnsignedType columnNum = 0; columnNum < columns; ++columnNum)
		{
			product[rowNum][columnNum] = matrix[rowNum][columnNum] * value;
		}
	}
	return product;
}

RealMatrix operator* (const RealMatrix& matrix,
	const Real& value)
{
	const UnsignedType rows = matrix.sizeRows();
	const UnsignedType columns = matrix.sizeColumns();
	RealMatrix product(rows, columns);

	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		for (UnsignedType columnNum = 0; columnNum < columns; ++columnNum)
		{
			product[rowNum][columnNum] = matrix[rowNum][columnNum] * value;
		}
	}
	return product;
}

RealMatrix operator+ (const RealMatrix& matrixFirst,
	const RealMatrix& matrixSecond)
{
	RealMatrix result = matrixFirst;
	const UnsignedType rowsFirst = matrixFirst.sizeRows();
	const UnsignedType columnsFirst = matrixFirst.sizeColumns();

	if (matrixSecond.sizeRows() != rowsFirst and
		matrixSecond.sizeColumns() != columnsFirst)
	{
		std::string msg = messageSizeMatricestNotEqual();
		ERROR(msg);
	}

	for (UnsignedType rowFirst = 0; rowFirst < rowsFirst; ++rowFirst)
	{
		for (UnsignedType columnFirst = 0; columnFirst < columnsFirst;
			++columnFirst)
		{
			result[rowFirst][columnFirst] +=
				matrixSecond[rowFirst][columnFirst];
		}
	}

	return result;
}

RealMatrix operator- (const RealMatrix& matrixFirst,
	const RealMatrix& matrixSecond)
{
	RealMatrix result = matrixFirst;
	const UnsignedType rowsFirst = matrixFirst.sizeRows();
	const UnsignedType columnsFirst = matrixFirst.sizeColumns();

	if (matrixSecond.sizeRows() != rowsFirst and
		matrixSecond.sizeColumns() != columnsFirst)
	{
		std::string msg = messageSizeMatricestNotEqual();
		ERROR(msg);
	}

	for (UnsignedType rowFirst = 0; rowFirst < rowsFirst; ++rowFirst)
	{
		for (UnsignedType columnFirst = 0; columnFirst < columnsFirst;
			++columnFirst)
		{
			result[rowFirst][columnFirst] -=
				matrixSecond[rowFirst][columnFirst];
		}
	}

	return result;
}
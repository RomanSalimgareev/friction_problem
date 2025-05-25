// This file implements functions from the operator_overloading.h file.
#include "operator_overloading.h"
#include "error_handling/error_handling.h"

using namespace MFE;

std::string messageSizeFirstLess()
{
	return "Vector size first < vector size second. ";
}

std::string messageSizeMatricestNotEqual()
{
	return "The sizes of the matrices are not equal. ";
}

std::string messageSizeNotCompatible()
{
	return "The sizes of the matrices are are not compatible. ";
}

///////////////////////////////////////////////////////////

RealVector MFE::operator* (const Real& value,
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

RealVector MFE::operator* (const RealVector& vec,
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

RealVector& MFE::operator+= (RealVector& vectorLhs,
	const RealVector& vectorRhs)
{
	const UnsignedType sizeSecond = vectorRhs.size();
	if (vectorLhs.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		vectorLhs[indexSecond] += vectorRhs[indexSecond];
	}
	return vectorLhs;
}

RealVector& MFE::operator-= (RealVector& vectorLhs,
	const RealVector& vectorRhs)
{
	const UnsignedType sizeSecond = vectorRhs.size();
	if (vectorLhs.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		vectorLhs[indexSecond] -= vectorRhs[indexSecond];
	}
	return vectorLhs;
}

RealVector MFE::operator+ (const RealVector& vectorLhs,
	const RealVector& vectorRhs)
{
	RealVector result = vectorLhs;
	const UnsignedType sizeSecond = vectorRhs.size();
	if (vectorLhs.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		result[indexSecond] += vectorRhs[indexSecond];
	}
	return result;
}

RealVector MFE::operator- (const RealVector& vectorLhs,
	const RealVector& vectorRhs)
{
	RealVector result = vectorLhs;
	const UnsignedType sizeSecond = vectorRhs.size();
	if (vectorLhs.size() < sizeSecond)
	{
		std::string msg = messageSizeFirstLess();
		ERROR(msg);
	}

	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		result[indexSecond] -= vectorRhs[indexSecond];
	}
	return result;
}


Real MFE::operator* (const RealVector& vectorLhs,
	const RealVector& vectorRhs)
{
	Real product = 0;
	const UnsignedType sizeSecond = vectorRhs.size();
	if (vectorLhs.size() != sizeSecond)
	{
		std::string msg = "The size of the vectors are not equal. ";
		ERROR(msg);
	}
	for (UnsignedType indexSecond = 0; indexSecond < sizeSecond; ++indexSecond)
	{
		product += vectorLhs[indexSecond] * vectorRhs[indexSecond];
	}
	return product;
}

RealMatrix MFE::matrixMultipOfVectors(const RealVector& vectorLhs,
	const RealVector& vectorRhs)
{
	const UnsignedType sizeLhs = vectorLhs.size();
	const UnsignedType sizeRhs = vectorRhs.size();
	if (sizeRhs == 0)
	{
		std::string msg = "The second vector is empty. ";
		ERROR(msg);
	}

	RealMatrix product(sizeLhs, sizeRhs);
	for (UnsignedType indexLhs = 0; indexLhs < sizeLhs; ++indexLhs)
	{
		for (UnsignedType indexRhs = 0; indexRhs < sizeRhs; ++indexRhs)
			product[indexLhs][indexRhs] = vectorLhs[indexLhs] *
				vectorRhs[indexRhs];
	}
	return product;
}

RealVector MFE::operator* (const RealMatrix& matrix,
	const RealVector& vec)
{
	const UnsignedType rowsMatrix = matrix.sizeRows();
	const UnsignedType columnsMatrix = matrix.sizeColumns();
	std::vector <Real> product(rowsMatrix);
	if (vec.size() < columnsMatrix)
	{
		std::string msg = "Vector size < columns of the matrix";
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

RealVector MFE::operator* (const RealVector& vec,
	const RealMatrix& matrix)
{
	const UnsignedType rowsMatrix = matrix.sizeRows();
	const UnsignedType columnsMatrix = matrix.sizeColumns();
	std::vector <Real> product(rowsMatrix);
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

///////////////////////////////////////////////////////////

RealMatrix MFE::operator* (const Real& value,
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

RealMatrix MFE::operator* (const RealMatrix& matrix,
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

RealMatrix MFE::operator+ (const RealMatrix& matrixLhs,
	const RealMatrix& matrixRhs)
{
	RealMatrix result = matrixLhs;
	const UnsignedType rowsFirst = matrixLhs.sizeRows();
	const UnsignedType columnsFirst = matrixLhs.sizeColumns();

	if (matrixRhs.sizeRows() != rowsFirst ||
		matrixRhs.sizeColumns() != columnsFirst)
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
				matrixRhs[rowFirst][columnFirst];
		}
	}

	return result;
}

RealMatrix MFE::operator- (const RealMatrix& matrixLhs,
	const RealMatrix& matrixRhs)
{
	RealMatrix result = matrixLhs;
	const UnsignedType rowsFirst = matrixLhs.sizeRows();
	const UnsignedType columnsFirst = matrixLhs.sizeColumns();

	if (matrixRhs.sizeRows() != rowsFirst ||
		matrixRhs.sizeColumns() != columnsFirst)
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
				matrixRhs[rowFirst][columnFirst];
		}
	}

	return result;
}

RealMatrix MFE::operator* (const RealMatrix& matrixLhs,
	const RealMatrix& matrixRhs)
{
	const UnsignedType rowsLhs = matrixLhs.sizeRows();
	const UnsignedType columnsLhs = matrixLhs.sizeColumns();

	const UnsignedType rowsRhs = matrixRhs.sizeRows();
	const UnsignedType columnsRhs = matrixRhs.sizeColumns();
	RealMatrix result(rowsLhs, columnsRhs);

	if (columnsLhs != rowsRhs)
	{
		std::string msg = messageSizeMatricestNotEqual();
		ERROR(msg);
	}

	for (UnsignedType rowLhs = 0; rowLhs < rowsLhs; ++rowLhs)
	{
		for (UnsignedType columnRhs = 0; columnRhs < columnsRhs; ++columnRhs)
		{
			for (UnsignedType rowRhs = 0; rowRhs < rowsRhs; ++rowRhs)
			{
				result[rowLhs][columnRhs] += matrixLhs[rowLhs][rowRhs] *
				matrixRhs[rowRhs][columnRhs];
			}
		}
	}

	return result;
}
// This file implements functions from the operator_overloading.h file.
#include "operator_overloading.h"
#include "error_handling.h"

using namespace MFE;

std::string messageSizeFirstLess()
{
	return "Vector size first < vector size second. ";
}

std::string messageSizeMatricestNotEqual()
{
	return "The sizes of the matrices are not equal. ";
}

template <typename T>
std::ostream& MFE::operator << (const Matrix<T>& matrix,
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
std::ostream& MFE::operator << (const std::vector<T>& vectorOutput,
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

RealVector MFE::operator* (const RealMatrix& matrix,
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

RealVector MFE::operator* (const RealVector& vec,
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

	if (matrixRhs.sizeRows() != rowsFirst &&
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

	if (matrixRhs.sizeRows() != rowsFirst &&
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
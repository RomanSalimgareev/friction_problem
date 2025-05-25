// This file is used in the solver_utils files.
// This file contains overloaded operators for the vector, the matrix class
// && the output operator for the vector && the matrix class.
#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <fstream>

#include <MFE_lib/types/matrix_type.h>

namespace MFE
{
	// Operators output
	// The operator for matrix output
	template <typename T>
	std::ostream& operator << (std::ostream& out,
		const Matrix<T>& matrix)
	{
		const UnsignedType rows = matrix.sizeRows();
		const UnsignedType columns = matrix.sizeColumns();

		for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
		{
			for (UnsignedType columnNum = 0; columnNum < columns; ++columnNum)
			{
				out << matrix[rowNum][columnNum] << " ";
			}
			out << "\n\n";
		}
		return out;
	}

	// The operator for vector output
	template <typename T>
	std::ostream& operator <<(std::ostream& out,
		const std::vector<T>& vectorOutput)
	{
		const UnsignedType size = vectorOutput.size();

		for (UnsignedType index = 0; index < size; ++index)
		{
			out << vectorOutput[index] << " ";
		}
		return out;
	}

	///////////////////////////////////////////////////////////

	// Operators for vector
	RealVector operator* (const Real& value,
		const RealVector& vec);

	RealVector operator* (const RealVector& vec,
		const Real& value);

	RealVector& operator+= (RealVector& vectorLhs,
		const RealVector& vectorRhs);

	RealVector& operator-= (RealVector& vectorLhs,
		const RealVector& vectorRhs);

	RealVector operator+ (const RealVector& vectorLhs,
		const RealVector& vectorRhs);

	RealVector operator- (const RealVector& vectorLhs,
		const RealVector& vectorRhs);

	// scalar multiplication
	Real operator* (const RealVector& vectorLhs,
		const RealVector& vectorRhs);

	//Multiplying a column vector by a row vector
	RealMatrix matrixMultipOfVectors(const RealVector& vectorLhs,
		const RealVector& vectorRhs);

	RealVector operator* (const RealMatrix& matrix,
		const RealVector& vec);

	RealVector operator* (const RealVector& vec,
		const RealMatrix& matrix);

	///////////////////////////////////////////////////////////

	// Operators for class Matrix
	RealMatrix operator* (const Real& value,
		const RealMatrix& matrix);

	RealMatrix operator* (const RealMatrix& matrix,
		const Real& value);

	RealMatrix operator+ (const RealMatrix& matrixLhs,
		const RealMatrix& matrixRhs);

	RealMatrix operator- (const RealMatrix& matrixLhs,
		const RealMatrix& matrixRhs);

	RealMatrix operator* (const RealMatrix& matrixLhs,
		const RealMatrix& matrixRhs);
}

#endif
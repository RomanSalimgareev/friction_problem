// This file is used in the solver_utils files.
// This file contains overloaded operators for the vector, the matrix class
// && the output operator for the vector && the matrix class.
#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <fstream>

#include "matrix_type.h"

namespace MFE
{
	// Operators output
	// The operator for matrix output
	template <typename T>
	std::ostream& operator << (const Matrix<T>& matrix, std::ostream& out);

	// The operator for vector output
	template <typename T>
	std::ostream& operator << (const std::vector<T>& vectorOutput,
		std::ostream& out);

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

	Real operator* (const RealVector& vectorLhs,
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
}

#endif 
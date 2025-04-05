// This file is used in the solver_utils files.
// This file contains overloaded operators for the vector, the matrix class
// and the output operator for the vector and the matrix class.
#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <fstream>

#include "Class_Matrix.h"

using Real = double;
using RealVector = std::vector<Real>;
using RealMatrix = Matrix<Real>;

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

RealVector& operator+= (RealVector& vectorFirst,
	const RealVector& vectorSecond);

RealVector& operator-= (RealVector& vectorFirst,
	const RealVector& vectorSecond);

RealVector operator+ (const RealVector& vectorFirst,
	const RealVector& vectorSecond);

RealVector operator- (const RealVector& vectorFirst,
	const RealVector& vectorSecond);

RealVector operator* (const RealMatrix& matrix,
	const RealVector& vec);

RealVector operator* (const RealVector& vec,
	const RealMatrix& matrix);

Real operator* (const RealVector& vectorFirst,
	const RealVector& vectorSecond);

///////////////////////////////////////////////////////////

// Operators for class Matrix
RealMatrix operator* (const Real& value,
	const RealMatrix& matrix);

RealMatrix operator* (const RealMatrix& matrix,
	const Real& value);

RealMatrix operator+ (const RealMatrix& matrixFirst,
	const RealMatrix& matrixSecond);

RealMatrix operator- (const RealMatrix& matrixFirst,
	const RealMatrix& matrixSecond);

#endif 
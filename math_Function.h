// This file is used in the initial_conditions files and and indirectly in the
// files dinamic_solver.
// This file contains mathematical functions : creating a Cholesky matrix for
// solving a static problem, transposing, and solving by the Gauss method with
// the choice of a leading element.

#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include "Class_Matrix.h"

using Real = double;
using RealVector = std::vector<Real>;
using RealMatrix = Matrix<Real>;

// Matrix Cholesky
// The Cholesky matrix is obtained by decomposing a matrix of coefficients; in the
// case of the finite element method, it is a stiffness matrix.
// matrix - the matrix you need.
RealMatrix createMatrixCholesky(const RealMatrix& matrix);

// Transpose matrix
// noTranspose - the untransposed matrix
RealMatrix transpose(const RealMatrix& noTranspose);

// Solving a system of linear equations by the Gauss method with the 
// choice of a leading element.
// An example of a system of linear equations : A * x = B.
// A - the matrixCoefficients, x - the vector is unlit, B - columnsFreeMembers
RealVector solveGauss(const RealMatrix& matrixCoefficients,
	const RealVector& columnsFreeMembers);

#endif 
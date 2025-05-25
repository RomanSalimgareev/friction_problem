// This file is used in the initial_conditions files and indirectly in the
// files dinamic_solver.
// This file contains mathematical functions : creating a Cholesky matrix for
// solving a static problem, transposing, and solving by the Gauss method with
// the choice of a leading element.

#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include <MFE_lib/types/matrix_type.h>
#include <math.h>
#include <cfloat> 

namespace MFE
{
	// Matrix Cholesky
	// The Cholesky matrix is obtained by decomposing a matrix of coefficients; in the
	// case of the finite element method, it is a stiffness matrix.
	// matrix - the matrix you need.
	MFE_LIB_EXPORT RealMatrix createMatrixCholesky(const RealMatrix& matrix);

	// Transpose matrix
	// noTranspose - the untransposed matrix
	MFE_LIB_EXPORT RealMatrix transpose(const RealMatrix& noTranspose);

	// The identity matrix
	MFE_LIB_EXPORT RealMatrix identityMatrix(const UnsignedType& rows);

	// Solving a system of linear equations by reflection method
	MFE_LIB_EXPORT RealVector solveReflectionMethod(const RealMatrix matrixCoefficients,
		const RealVector freeTerms);
}

#endif 
#include "math_Function.h"

// Matrix Cholesky
RealMatrix createMatrixCholesky(const RealMatrix& matrix)
{
	UnsignedType n = matrix.sizeRows();
	RealMatrix L(n, n);

	for (UnsignedType i = 0; i < n; ++i) {
		for (UnsignedType j = 0; j <= i; ++j) {
			Real sum = 0;
			if (j == i) {
				for (UnsignedType k = 0; k < j; ++k) {
					sum += pow(L[j][k], 2);
				}
				L[j][j] = sqrt(matrix[j][j] - sum);
			}
			else {
				for (UnsignedType k = 0; k < j; ++k) {
					sum += (L[i][k] * L[j][k]);
				}
				L[i][j] = (matrix[i][j] - sum) / L[j][j];
			}
		}
	}

	return L;
}

// Transpose matrix
template <typename T>
Matrix<T> transpose(const Matrix<T>& noTranspose)
{
	UnsignedType r = noTranspose.sizeRows();
	UnsignedType c = noTranspose.sizeColumns();

	Matrix<T> new_matrix(c, r);
	for (UnsignedType i = 0; i < c; ++i)
	{
		for (UnsignedType j = 0; j < r; ++j)
		{
			new_matrix[i][j] = noTranspose[j][i];
		}
	}
	return new_matrix;
}

// Solving a system of linear equations by the Gauss method with
// the choice of a leading element
RealVector solveGauss(const RealMatrix& matrixCoefficients,
	const RealVector& columnsFreeMembers)
{
	const UnsignedType r = matrixCoefficients.sizeRows();
	const UnsignedType c = matrixCoefficients.sizeColumns();
	RealMatrix A = matrixCoefficients;
	RealVector B = columnsFreeMembers;
	RealVector result(r);

	for (UnsignedType i = 0; i < c; ++i)
	{
		UnsignedType maxIndex = i;
		for (UnsignedType j = i; j < r; ++j)
		{
			if (abs(A[j][i]) > abs(A[maxIndex][i]))
			{
				maxIndex = j;
			}
		}

		std::swap(A[i], A[maxIndex]);
		std::swap(B[i], B[maxIndex]);

		for (UnsignedType k = i; k < r; ++k)
		{
			Real firstCoefficient = A[k][i];
			if (k == i)
				B[k] = B[k] / firstCoefficient;
			else
				B[k] -= B[i] * firstCoefficient;

			for (UnsignedType m = i; m < c; ++m)
			{
				if (k == i)
					A[k][m] = A[k][m] / firstCoefficient;
				else
					A[k][m] -= A[i][m] * firstCoefficient;
			}
		}
	}

	for (UnsignedType i = r; i-- > 0; )
	{
		Real sum = 0;
		for (UnsignedType j = i + 1; j < r; ++j)
		{
			sum += result[j] * A[i][j];
		}
		result[i] = B[i] - sum;
	}

	return result;
}
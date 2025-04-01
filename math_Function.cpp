#include "math_Function.h"

// Matrix Cholesky
RealMatrix createMatrixCholesky(const RealMatrix& matrixStiffness)
{
	UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	if (rowsStiffness != matrixStiffness.sizeColumns())
	{
		std::string msg = "The matrix is not square. ";
		ASSERT(rowsStiffness == matrixStiffness.sizeColumns(), msg);
		ERROR(msg);
	}

	RealMatrix matrixCholesky(rowsStiffness, rowsStiffness);

	for (UnsignedType i = 0; i < rowsStiffness; ++i) {
		for (UnsignedType j = 0; j <= i; ++j) {
			Real sum = 0;
			if (j == i) {
				for (UnsignedType k = 0; k < j; ++k) {
					sum += pow(matrixCholesky[j][k], 2);
				}
				matrixCholesky[j][j] = sqrt(matrixStiffness[j][j] - sum);
			}
			else {
				for (UnsignedType k = 0; k < j; ++k)
					sum += (matrixCholesky[i][k] * matrixCholesky[j][k]);

				if (matrixCholesky[j][j] <= DBL_EPSILON)
				{
					std::string msg = messageDivideZero();
					ERROR(msg);
				}

				matrixCholesky[i][j] = 
					(matrixStiffness[i][j] - sum) / matrixCholesky[j][j];
			}
		}
	}

	return matrixCholesky;
}

// Transpose matrix
RealMatrix transpose(const RealMatrix& noTranspose)
{
	UnsignedType rows = noTranspose.sizeRows();
	UnsignedType columns = noTranspose.sizeColumns();

	RealMatrix new_matrix(columns, rows);
	for (UnsignedType i = 0; i < columns; ++i)
	{
		for (UnsignedType j = 0; j < rows; ++j)
		{
			new_matrix[i][j] = noTranspose[j][i];
		}
	}
	return new_matrix;
}

// Solving a system of linear equations by the Gauss method with
// the choice of a leading element
RealVector solveGauss(const RealMatrix& matrixCoefficients,
	const RealVector& columnFreeMembers)
{
	const UnsignedType rows = matrixCoefficients.sizeRows();
	const UnsignedType columns = matrixCoefficients.sizeColumns();
	RealMatrix A = matrixCoefficients;
	RealVector B = columnFreeMembers;
	RealVector result(rows, 0.0);
	if (columnFreeMembers.size() != rows)
	{
		std::string msg = "Vector size < matrix rows. ";
		ASSERT(rows == columnFreeMembers.size(), msg);
		ERROR(msg);
	}

	for (UnsignedType i = 0; i < columns; ++i)
	{
		UnsignedType maxIndex = i;
		for (UnsignedType j = i; j < rows; ++j)
		{
			if (abs(A[j][i]) > abs(A[maxIndex][i]))
			{
				maxIndex = j;
			}
		}

		std::swap(A[i], A[maxIndex]);
		std::swap(B[i], B[maxIndex]);

		for (UnsignedType k = i; k < rows; ++k)
		{
			Real firstCoefficient = A[k][i];
			if (firstCoefficient <= DBL_EPSILON)
			{
				std::string msg = messageDivideZero();
				ERROR(msg);
			}

			if (k == i)
				B[k] = B[k] / firstCoefficient;
			else
				B[k] -= B[i] * firstCoefficient;

			for (UnsignedType m = i; m < columns; ++m)
			{
				if (k == i)
					A[k][m] = A[k][m] / firstCoefficient;
				else
					A[k][m] -= A[i][m] * firstCoefficient;
			}
		}
	}

	for (UnsignedType i = rows; i-- > 0; )
	{
		Real sum = 0;
		for (UnsignedType j = i + 1; j < rows; ++j)
		{
			sum += result[j] * A[i][j];
		}
		result[i] = B[i] - sum;
	}

	return result;
}
#include "math_functions.h"
#include "error_handling.h"

using namespace MFE;

// Matrix Cholesky
RealMatrix MFE::createMatrixCholesky(const RealMatrix& matrixStiffness)
{
	UnsignedType matrixSize = matrixStiffness.sizeRows();
	if (matrixSize != matrixStiffness.sizeColumns())
	{
		std::string msg = "The matrix is not square. ";
		ASSERT(matrixSize == matrixStiffness.sizeColumns(), msg);
		ERROR(msg);
	}

	RealMatrix matrixCholesky(matrixSize, matrixSize);

	for (UnsignedType rowNum = 0; rowNum < matrixSize; ++rowNum) 
	{
		for (UnsignedType columnNum = 0; columnNum <= rowNum; ++columnNum) 
		{
			Real sum = 0;
			if (columnNum == rowNum) 
			{
				for (UnsignedType k = 0; k < columnNum; ++k) 
				{
					sum += pow(matrixCholesky[columnNum][k], 2);
				}
				matrixCholesky[columnNum][columnNum] = 
					sqrt(matrixStiffness[columnNum][columnNum] - sum);
			}
			else 
			{
				for (UnsignedType k = 0; k < columnNum; ++k)
					sum += (matrixCholesky[rowNum][k] * matrixCholesky[columnNum][k]);

				if (matrixCholesky[columnNum][columnNum] <= DBL_EPSILON)
				{
					std::string msg = messageDivideZero();
					ERROR(msg);
				}

				matrixCholesky[rowNum][columnNum] = 
					(matrixStiffness[rowNum][columnNum] - sum) /
						matrixCholesky[columnNum][columnNum];
			}
		}
	}

	return matrixCholesky;
}

// Transpose matrix
RealMatrix MFE::transpose(const RealMatrix& noTranspose)
{
	UnsignedType rows = noTranspose.sizeRows();
	UnsignedType columns = noTranspose.sizeColumns();

	RealMatrix new_matrix(columns, rows);
	for (UnsignedType columnNum = 0; columnNum < columns; ++columnNum)
	{
		for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
		{
			new_matrix[columnNum][rowNum] = noTranspose[rowNum][columnNum];
		}
	}
	return new_matrix;
}

// Solving a system of linear equations by the Gauss method with
// the choice of a leading element
RealVector MFE::solveGauss(const RealMatrix& matrixCoefficients,
	const RealVector& columnFreeMembers)
{
	const UnsignedType rows = matrixCoefficients.sizeRows();
	const UnsignedType columns = matrixCoefficients.sizeColumns();
	RealMatrix coeffMatrix = matrixCoefficients;
	RealVector freeTerms = columnFreeMembers;
	RealVector solution(rows, 0.0);
	if (columnFreeMembers.size() != rows)
	{
		std::string msg = "Vector size < matrix rows. ";
		ASSERT(rows == columnFreeMembers.size(), msg);
		ERROR(msg);
	}

	for (UnsignedType pivotColumn = 0; pivotColumn < columns; ++pivotColumn)
	{
		UnsignedType pivotRow = pivotColumn;
		for (UnsignedType rowNum = pivotColumn; rowNum < rows; ++rowNum)
		{
			if (abs(coeffMatrix[rowNum][pivotColumn]) >
				abs(coeffMatrix[pivotRow][pivotColumn]))
			{
				pivotRow = rowNum;
			}
		}

		std::swap(coeffMatrix[pivotColumn], coeffMatrix[pivotRow]);
		std::swap(freeTerms[pivotColumn], freeTerms[pivotRow]);

		for (UnsignedType rowToEliminate = pivotColumn; rowToEliminate < rows;
			++rowToEliminate)
		{
			Real pivotValue = coeffMatrix[rowToEliminate][pivotColumn];
			if (pivotValue <= DBL_EPSILON)
			{
				std::string msg = messageDivideZero();
				ERROR(msg);
			}

			if (rowToEliminate == pivotColumn)
			{
				freeTerms[rowToEliminate] = freeTerms[rowToEliminate] /
					pivotValue;
			}
			else
			{
				freeTerms[rowToEliminate] -= freeTerms[pivotColumn] *
					pivotValue;
			}

			for (UnsignedType col = pivotColumn; col < columns; ++col)
			{
				if (rowToEliminate == pivotColumn)
				{
					coeffMatrix[rowToEliminate][col] =
						coeffMatrix[rowToEliminate][col] / pivotValue;
				}
				else
				{
					coeffMatrix[rowToEliminate][col] -=
						coeffMatrix[pivotColumn][col] * pivotValue;
				}
			}
		}
	}

	for (UnsignedType rowNum = rows; rowNum-- > 0; )
	{
		Real sum = 0;
		for (UnsignedType columnNum = rowNum + 1; columnNum < rows; ++columnNum)
		{
			sum += solution[columnNum] * coeffMatrix[rowNum][columnNum];
		}
		solution[rowNum] = freeTerms[rowNum] - sum;
	}

	return solution;
}
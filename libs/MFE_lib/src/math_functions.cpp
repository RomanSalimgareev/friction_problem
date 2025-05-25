#include <MFE_lib/math_functions.h>

#include "error_handling/error_handling.h"
#include "operator_overloading/operator_overloading.h"

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

				Real value = matrixStiffness[columnNum][columnNum] - sum;
				if (value < 0) {
					std::string msg = "Matrix is not positive definite.";
					ERROR(msg);
				}

				matrixCholesky[columnNum][columnNum] = 
					sqrt(matrixStiffness[columnNum][columnNum] - sum);
			}
			else 
			{
				for (UnsignedType k = 0; k < columnNum; ++k)
					sum += (matrixCholesky[rowNum][k] * matrixCholesky[columnNum][k]);

				if (abs(matrixCholesky[columnNum][columnNum]) <= DBL_EPSILON)
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

RealMatrix MFE::identityMatrix(const UnsignedType& rows)
{
	RealMatrix identity(rows, rows);
	for (UnsignedType row = 0; row < rows; ++row)
		identity[row][row] = 1;

	return identity;
}

RealVector MFE::solveReflectionMethod(RealMatrix matrixCoeffs, 
	RealVector freeTerms)
{
	UnsignedType rows = matrixCoeffs.sizeRows();
	UnsignedType columns = matrixCoeffs.sizeColumns();
	if (rows != columns)
	{
		std::string msg = "The matrix is not square. ";
		ASSERT(rows == columns, msg);
		ERROR(msg);
	}

	if (rows != freeTerms.size())
	{
		std::string msg = "Vector size < matrix rows. ";
		ASSERT(rows == freeTerms.size(), msg);
		ERROR(msg);
	}

	RealVector solution(columns, 0);
	RealVector ort(columns, 0);

	for (UnsignedType row = 0; row < rows - 1; ++row)
	{
		// defining the ort
		RealVector z(columns, 0.0);
		z[row] = 1.0;

		RealVector y(rows, 0);
		for (UnsignedType elemColumn = row; elemColumn < rows; ++elemColumn)
		{
			// we define a vector consisting of the coefficients of the column
			y[elemColumn] = matrixCoeffs[elemColumn][row];
		}
		
		// we find alpha by defining the 2 norm y and z
		Real normY = std::sqrt(y * y);
		Real normZ = std::sqrt(z * z);
		double sign = -1;
		if (y[row] >= 0)
			sign = 1;
		Real alpha = -sign * normY / normZ;

		// find vector w
		RealVector diff = y - alpha * z;
		Real normDenominator = std::sqrt(diff * diff);
		RealVector w = (1 / normDenominator) * diff;

		RealMatrix identity = identityMatrix(rows);
		
		// The search for the matrix U is omitted, and a new matrix of 
		// coefficients and free terms is immediately searched for.
		//RealMatrix U = identity - 2 * matrixMultipOfVectors(w, w);

		RealVector wOnMatrixCoeffs = w * matrixCoeffs;
		// because a vector in c++ is represented in the dimension 1xN
		matrixCoeffs = matrixCoeffs - 2 * matrixMultipOfVectors(w, wOnMatrixCoeffs);

		Real wOnFreeTerms = w * freeTerms;
		freeTerms = freeTerms - 2 * w * wOnFreeTerms;
	}

	// Reverse gear
	for (UnsignedType rowNum = rows; rowNum-- > 0; )
	{
		Real sum = 0;
		for (UnsignedType columnNum = rowNum + 1; columnNum < rows; ++columnNum)
		{
			sum += solution[columnNum] * matrixCoeffs[rowNum][columnNum];
		}
		solution[rowNum] = (freeTerms[rowNum] - sum);

		if (abs(matrixCoeffs[rowNum][rowNum]) <= DBL_EPSILON)
		{
			std::string msg = messageDivideZero();
			ERROR(msg);
		}
		solution[rowNum] = solution[rowNum] / matrixCoeffs[rowNum][rowNum];
	}
	
	return solution;
}
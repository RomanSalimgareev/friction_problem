#include "matrix_MFE.h"

// The shape function
Real shapeFunction(const Array3D& locPoint, const Array3D& quadPoint)
{
	Real result = 1.0 / 8.0 * (1.0 + locPoint[0] * quadPoint[0]) *
		(1.0 + locPoint[1] * quadPoint[1]) *
		(1.0 + locPoint[2] * quadPoint[2]);
	return result;
}

// The derivative of the shape function by ksi
Real dShapeFuncKsi(const Array3D& locPoint,
	const Real& quadraticPointEtta, const Real& quadraticPointPsi,
	const Real& length)
{
	Real dKsi = 1.0 / 4.0 * locPoint[0] / length *
		(1.0 + locPoint[1] * quadraticPointEtta) *
		(1.0 + locPoint[2] * quadraticPointPsi);

	return dKsi;
}

// The derivative of the shape function by etta
Real dShapeFuncEtta(const Array3D& locPoint,
	const Real& quadraticPointKsi, const Real& quadraticPointPsi,
	const Real& width)
{
	Real dEtta = 1.0 / 4.0 * (1.0 + locPoint[0] * quadraticPointKsi) *
		locPoint[1] / width * (1.0 + locPoint[2] * quadraticPointPsi);

	return dEtta;
}

// The derivative of the shape function by psi
Real dShapeFuncPsi(const Array3D& locPoint,
	const Real& quadraticPointKsi, const Real& quadraticPointEtta,
	const Real& higth)
{
	Real dPsi = 1.0 / 4.0 * (1.0 + locPoint[0] * quadraticPointKsi) *
		(1.0 + locPoint[1] * quadraticPointEtta) * locPoint[2] / higth;

	return dPsi;
}

// This function creates a matrix of local coordinates
RealMatrix getLocalCoordinate()
{
	RealMatrix matrix = { {1.0 ,1.0 ,-1.0, -1.0, 1.0, 1.0, -1.0, -1.0 },
		{ -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0 },
		{ -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0 } };

	return matrix;
}

// Its function creates a elastic constants matrix
RealMatrix makeMatrixElConst(const UnsignedType& rows,
	const UnsignedType& columns, const Real& modulusElastic,
	const Real& coeffPuasson)
{
	RealMatrix matrix(rows, columns);
	for (UnsignedType i = 0; i < rows; ++i)
	{
		for (UnsignedType j = 0; j < columns; ++j)
		{
			if (i < 3)
			{
				if (i == j)
					matrix[i][j] = modulusElastic * (1.0 - coeffPuasson) /
					((1.0 + coeffPuasson) *
						(1.0 - 2.0 * coeffPuasson));
				else if (j < 3)
					matrix[i][j] = modulusElastic * coeffPuasson /
					((1.0 + coeffPuasson) *
						(1.0 - 2.0 * coeffPuasson));
			}
			else
			{
				if (i == j)
					matrix[i][j] = modulusElastic /
					(2.0 * (1.0 + coeffPuasson));
			}
		}
	}
	return matrix;
}

// Its function creates a matrix of quadratic points
RealMatrix makeMatrixQuadPoints(const UnsignedType& rows,
	const UnsignedType& columns, const RealMatrix& localCoordinate)
{
	RealMatrix matrix(rows, columns);
	for (UnsignedType i = 0; i < rows; ++i)
	{
		for (UnsignedType j = 0; j < columns; ++j)
		{
			matrix[i][j] = sqrt(3.0) / 3.0 * localCoordinate[i][j];
		}
	}
	return matrix;
}

// There "bt" - the transposed matrix b, where matrix b is the
// matrix of differentiation from the finite element method;
// D is the matrix of elastic constants, i.e. the matrix ElasticConst.
// Its matrix product "bt" and "matrix of elastic constants".
RealMatrix makeMatrixBtD(const RealMatrix& bTranspose,
	const RealMatrix& elasticConstMatrix)
{
	const UnsignedType btColumns = bTranspose.sizeColumns();
	const UnsignedType elasticColumns = elasticConstMatrix.sizeColumns();
	RealMatrix result(btColumns, elasticColumns);

	for (UnsignedType i = 0; i < btColumns; ++i)
	{
		for (UnsignedType j = 0; j < elasticColumns; ++j)
		{
			for (UnsignedType k = 0; k < elasticColumns; ++k)
			{
				result[i][j] += bTranspose[k][i] * elasticConstMatrix[k][j];
			}
		}
	}
	return result;
}

// This function creates a diagonal mass matrix.
RealMatrix makeMatrixMassDiag(const UnsignedType& size,
	const Real& dencity, const Real& length,
	const Real& width, const Real& heigth)
{
	RealMatrix matrix(size, size);
	UnsignedType nNodes = size / 3;
	Real mass = dencity * length * width * heigth;
	for (UnsignedType i = 0; i < size; ++i)
	{
		matrix[i][i] = mass / nNodes;
	}
	return matrix;
}

// This function creates a joint mass matrix.
RealMatrix makeMatrixMassJoint(const UnsignedType& size,
	const Real& dencity, const Real& length, const Real& width,
	const Real& heigth)
{
	const Real detMatrixJacobian = length * width * heigth / 8.0;

	RealMatrix locCoord = getLocalCoordinate();

	const UnsignedType m = locCoord.sizeRows();

	RealMatrix quadPoints = makeMatrixQuadPoints(m, 8, locCoord);
	const UnsignedType n = quadPoints.sizeColumns();
	RealMatrix matrixMass(size, size);
	for (UnsignedType i = 0; i < n; ++i)
	{
		for (UnsignedType j = 0; j < n; ++j)
		{
			for (UnsignedType k = 0; k < n; ++k)
			{
				// locCoord[0][i] - ksi, locCoord[1][i] - etta, 
				// locCoord[2][i] - psi
				Array3D locPoint{ locCoord[0][i], locCoord[1][i],
					locCoord[2][i] };

				// quadPoints[0][k] - quadratic point ksi, 
				// quadPoints[1][k] - quadratic point etta, 
				// quadPoints[2][k] - quadratic point psi
				Array3D quadPoint{ quadPoints[0][k], quadPoints[1][k],
					quadPoints[2][k] };

				Real ShapeFuncI = shapeFunction(locPoint, quadPoint);

				Real shapeFuncJ = shapeFunction(locPoint, quadPoint);

				matrixMass[3 * i][3 * j] += ShapeFuncI * shapeFuncJ;

				matrixMass[3 * i + 1][3 * j + 1] += ShapeFuncI * shapeFuncJ;

				matrixMass[3 * i + 2][3 * j + 2] += ShapeFuncI * shapeFuncJ;
			}

			matrixMass[3 * i][3 * j] *= detMatrixJacobian * dencity;

			matrixMass[3 * i + 1][3 * j + 1] *= detMatrixJacobian * dencity;

			matrixMass[3 * i + 2][3 * j + 2] *= detMatrixJacobian * dencity;
		}
	}
	return matrixMass;
}

// This function creates a matrix stiffness.
RealMatrix makeMatrixStiffness(const Real& length,
	const Real& width, const Real& heigth, const Real& modulusEl,
	const Real& coeffPuasson)
{
	const Real detMatrixJacobian = length * width * heigth / 8.0;

	RealMatrix locCoord = getLocalCoordinate();

	RealMatrix ElasticConst = makeMatrixElConst(6, 6, modulusEl,
		coeffPuasson);
	const UnsignedType rowsElastic = ElasticConst.sizeRows();

	const UnsignedType rowsLocCord = locCoord.sizeRows();
	RealMatrix quadPoints = 
		makeMatrixQuadPoints(rowsLocCord, 8, locCoord);
	const UnsignedType columnsQuad = quadPoints.sizeColumns();

	const UnsignedType sizeStiffness = rowsLocCord * columnsQuad;
	RealMatrix result(sizeStiffness, sizeStiffness);
	for (UnsignedType l = 0; l < columnsQuad; l++)
	{
		UnsignedType g = 0;
		Array3D locPoint{ locCoord[0][g], locCoord[1][g],
					locCoord[2][g] };

		// construction of the differentiation matrix
		RealMatrix matrixDifferentiation(rowsElastic, sizeStiffness);
		for (UnsignedType k = 0; k < sizeStiffness - 2; k += 3)
		{
			locPoint = { locCoord[0][g], locCoord[1][g],
					locCoord[2][g] };

			matrixDifferentiation[0][k] = 
				dShapeFuncKsi(locPoint, quadPoints[1][l],
				quadPoints[2][l], length);

			matrixDifferentiation[1][k + 1] = 
				dShapeFuncEtta(locPoint, quadPoints[0][l],
				quadPoints[2][l], width);

			matrixDifferentiation[2][k + 2] = 
				dShapeFuncPsi(locPoint, quadPoints[0][l],
				quadPoints[1][l], heigth);
			g++;
		}

		g = 0;
		for (UnsignedType k = 0; k < sizeStiffness - 2; k += 3)
		{
			locPoint = { locCoord[0][g], locCoord[1][g],
			locCoord[2][g] };

			matrixDifferentiation[3][k] = 
				dShapeFuncEtta(locPoint, quadPoints[0][l],
				quadPoints[2][l], width);

			matrixDifferentiation[3][k + 1] = 
				dShapeFuncKsi(locPoint, quadPoints[1][l],
				quadPoints[2][l], length);
			g++;
		}

		g = 0;
		for (UnsignedType k = 0; k < sizeStiffness - 2; k += 3)
		{
			locPoint = { locCoord[0][g], locCoord[1][g],
			locCoord[2][g] };

			matrixDifferentiation[4][k + 1] = 
				dShapeFuncPsi(locPoint, quadPoints[0][l],
				quadPoints[1][l], heigth);

			matrixDifferentiation[4][k + 2] = 
				dShapeFuncEtta(locPoint, quadPoints[0][l],
				quadPoints[2][l], width);
			g++;
		}

		g = 0;
		for (UnsignedType k = 0; k < sizeStiffness - 2; k += 3)
		{
			locPoint = { locCoord[0][g], locCoord[1][g],
			locCoord[2][g] };

			matrixDifferentiation[5][k] = 
				dShapeFuncPsi(locPoint, quadPoints[0][l],
				quadPoints[1][l], heigth);
			matrixDifferentiation[5][k + 2] = 
				dShapeFuncKsi(locPoint, quadPoints[1][l],
				quadPoints[2][l], length);
			g++;
		}

		RealMatrix transposeMatrix = 
			makeMatrixBtD(matrixDifferentiation, ElasticConst);

		// Obtaining the final stiffness matrix
		for (UnsignedType i = 0; i < sizeStiffness; ++i)
		{
			for (UnsignedType j = 0; j < sizeStiffness; ++j)
			{
				Real product = 0;
				for (UnsignedType k = 0; k < rowsElastic; ++k)
				{
					product += transposeMatrix[i][k] *
						matrixDifferentiation[k][j];
				}
				result[i][j] += product * detMatrixJacobian;
			}
		}
	}

	return result;
}
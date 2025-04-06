#include "matrix_MFE.h"

std::string messageFuncNotCalculate()
{
	return "The function did not calculate the value";
}

void checkIncorrectValue(Properties propertyFiniteElement, Real& value)
{
	switch (propertyFiniteElement)
	{
	case PROPERTIES_MODULUS_ELASTIC:
	{
		while (value < MIN_DENCITY || value > MAX_DENCITY)
		{
			std::cout << "Invalid finite element modulus of elasticity, "
				"input the value again: \n";
			std::cin >> value;
		}
		break;
	}
	case PROPERTIES_POISSONS_RATIO:
	{
		while (value <= DBL_EPSILON || value >= LIM_POISSONS_RATIO)
		{
			std::cout << "Invalid finite element poission's ratio, "
				"input the value again: \n";
			std::cin >> value;
		}
		break;
	}
	case PROPERTIES_DENCITY:
	{
		while (value < MIN_DENCITY || value > MAX_DENCITY)
		{
			std::cout << "Invalid finite element dencity, "
				"input the value again: \n";
			std::cin >> value;
		}
		break;
	}
	case PROPERTIES_LENGTH:
	{
		while (value < MIN_SIZE_FINITE_ELEMENT)
		{
			std::cout << "Invalid finite element length, "
				"input the value again: \n";
			std::cin >> value;
		}
		break;
	}
	case PROPERTIES_WIDTH:
	{
		while (value < MIN_SIZE_FINITE_ELEMENT)
		{
			std::cout << "Invalid finite element width, "
				"input the value again: \n";
			std::cin >> value;
		}
		break;
	}
	case PROPERTIES_HEIGTH:
	{
		while (value < MIN_SIZE_FINITE_ELEMENT)
		{
			std::cout << "Invalid finite element heigth, "
				"input the value again: \n";
			std::cin >> value;
		}
		break;
	}
	default:
	{
		std::cout << "The material property is not specified in the end "
			"element field.Make changes to the program. \n";
		break;
	}
	}
}

// Input material properties manually
void setMaterialProperties(FiniteElement& finiteElement)
{
	std::cout << "Input modulus elastic: " << "\n";
	std::cin >> finiteElement.modulusElastic;
	checkIncorrectValue(PROPERTIES_MODULUS_ELASTIC,
		finiteElement.modulusElastic);

	std::cout << "Input coefficient Puasson: " << "\n";
	std::cin >> finiteElement.poissonRatio;
	checkIncorrectValue(PROPERTIES_POISSONS_RATIO,
		finiteElement.poissonRatio);

	std::cout << "Input dencity: " << "\n";
	std::cin >> finiteElement.dencity;
	checkIncorrectValue(PROPERTIES_DENCITY, finiteElement.dencity);

	std::cout << "Input length final element: " << "\n";
	std::cin >> finiteElement.length;
	checkIncorrectValue(PROPERTIES_LENGTH, finiteElement.length);

	std::cout << "Input width final element: " << "\n";
	std::cin >> finiteElement.width;
	checkIncorrectValue(PROPERTIES_WIDTH, finiteElement.width);

	std::cout << "Input heigth final element: " << "\n";
	std::cin >> finiteElement.heigth;
	checkIncorrectValue(PROPERTIES_HEIGTH, finiteElement.heigth);
}

// The choice of setting material properties: by default || manually
void chooseMaterialProperties(FiniteElement& finiteElement)
{
	char choiceParameters = 'n';
	std::cout << "Do you want to enter the parameters manually? (y/n) \n";
	std::cin >> choiceParameters;
	if (choiceParameters == 'y' || choiceParameters == 'Y')
		setMaterialProperties(finiteElement);
	else
		std::cout << "the parameters are selected by default: \n" <<
		"modulus elastic = " << finiteElement.modulusElastic << "; \n" <<
		"poisson's ratio = " << finiteElement.poissonRatio << "; \n" <<
		"dencity = " << finiteElement.dencity << "; \n" <<
		"length = " << finiteElement.length << "; \n" <<
		"width = " << finiteElement.width << "; \n" <<
		"heigth = " << finiteElement.heigth << "; \n";
}

// The shape function
Real shapeFunction(const Array3D& locPoint, const Array3D& quadPoint)
{
	Real result = 0.0;
	const bool isCorrectData = locPoint.size() == 3 &&
		quadPoint.size() == 3;
	if (isCorrectData)
	{
		result = 1.0 / 8.0 * (1.0 + locPoint[0] * quadPoint[0]) *
			(1.0 + locPoint[1] * quadPoint[1]) *
			(1.0 + locPoint[2] * quadPoint[2]);
	}
	else
	{
		std::string msg = messageFuncNotCalculate();
		ASSERT(isCorrectData, msg);
		WARNING(msg);
	}

	return result;
}

// The derivative of the shape function by ksi
Real dShapeFuncKsi(const Array3D& locPoint,
	const Real& quadraticPointEtta, const Real& quadraticPointPsi,
	const Real& length)
{
	Real dKsi = 0.0;
	const bool isCorrectData = locPoint.size() == 3 &&
		length >= MIN_SIZE_FINITE_ELEMENT;
	if (isCorrectData)
	{
		dKsi = 1.0 / 4.0 * locPoint[0] / length *
			(1.0 + locPoint[1] * quadraticPointEtta) *
			(1.0 + locPoint[2] * quadraticPointPsi);
	}
	else
	{
		std::string msg = messageFuncNotCalculate();
		ASSERT(isCorrectData, msg);
		WARNING(msg);
	}

	return dKsi;
}

// The derivative of the shape function by etta
Real dShapeFuncEtta(const Array3D& locPoint,
	const Real& quadraticPointKsi, const Real& quadraticPointPsi,
	const Real& width)
{
	Real dEtta = 0.0;
	const bool isCorrectData = locPoint.size() == 3 &&
		width >= MIN_SIZE_FINITE_ELEMENT;
	if (isCorrectData)
	{
		dEtta = 1.0 / 4.0 * (1.0 + locPoint[0] * quadraticPointKsi) *
			locPoint[1] / width * (1.0 + locPoint[2] * quadraticPointPsi);
	}
	else
	{
		std::string msg = messageFuncNotCalculate();
		ASSERT(isCorrectData, msg);
		WARNING(msg);
	}

	return dEtta;
}

// The derivative of the shape function by psi
Real dShapeFuncPsi(const Array3D& locPoint,
	const Real& quadraticPointKsi, const Real& quadraticPointEtta,
	const Real& heigth)
{
	Real dPsi = 0.0;
	const bool isCorrectData = locPoint.size() == 3 &&
		heigth >= MIN_SIZE_FINITE_ELEMENT;
	if (isCorrectData)
	{
		dPsi = 1.0 / 4.0 * (1.0 + locPoint[0] * quadraticPointKsi) *
			(1.0 + locPoint[1] * quadraticPointEtta) * locPoint[2] / heigth;
	}
	else
	{
		std::string msg = messageFuncNotCalculate();
		ASSERT(isCorrectData, msg);
		WARNING(msg);
	}

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
	const Real& poissonRatio)
{
	RealMatrix matrix(rows, columns);
	if (poissonRatio >= LIM_POISSONS_RATIO)
	{
		std::string msg = "The Poisson's ratio is greater than || "
			"equal to 0.5. ";
		ASSERT(poissonRatio < LIM_POISSONS_RATIO, msg);
		ERROR(msg);
	}

	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		for (UnsignedType columnNum = 0; columnNum < columns; ++columnNum)
		{
			if (rowNum < 3)
			{
				if (rowNum == columnNum)
				{
					matrix[rowNum][columnNum] =
						modulusElastic * (1.0 - poissonRatio);

					matrix[rowNum][columnNum] =
						matrix[rowNum][columnNum] / (1.0 + poissonRatio);

					matrix[rowNum][columnNum] = 
						matrix[rowNum][columnNum] / (1.0 - 2.0 * poissonRatio);
				}
				else if (columnNum < 3)
				{
					matrix[rowNum][columnNum] = modulusElastic * poissonRatio;

					matrix[rowNum][columnNum] = 
						matrix[rowNum][columnNum] / (1.0 + poissonRatio);

					matrix[rowNum][columnNum] =
						matrix[rowNum][columnNum] / (1.0 - 2.0 * poissonRatio);
				}
			}
			else
			{
				if (rowNum == columnNum)
				{
					matrix[rowNum][columnNum] =
						modulusElastic / (2.0 * (1.0 + poissonRatio));
				}
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
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		for (UnsignedType columnNum = 0; columnNum < columns; ++columnNum)
		{
			matrix[rowNum][columnNum] = 
				sqrt(3.0) / 3.0 * localCoordinate[rowNum][columnNum];
		}
	}
	return matrix;
}

// There "bt" - the transposed matrix b, where matrix b is the
// matrix of differentiation from the finite element method;
// D is the matrix of elastic constants, i.e. the matrix ElasticConst.
// Its matrix product "bt" && "matrix of elastic constants".
RealMatrix makeMatrixBtD(const RealMatrix& bTranspose,
	const RealMatrix& elasticConstMatrix)
{
	const UnsignedType bTransposeRows = bTranspose.sizeRows();
	const UnsignedType elasticRows = elasticConstMatrix.sizeRows();
	if (bTranspose.sizeColumns() != elasticRows)
	{
		std::string msg = "Matrix size mismatch : cols(A) != rows(B). ";
		ERROR(msg);
	}

	if (elasticRows != elasticConstMatrix.sizeColumns())
	{
		std::string msg = "The matrix of elastic constants is not square";
		ERROR(msg);
	}

	RealMatrix result(bTransposeRows, elasticRows);
	for (UnsignedType rowBtNum = 0; rowBtNum < bTransposeRows; ++rowBtNum)
	{
		for (UnsignedType columnElasticNum = 0; columnElasticNum < elasticRows;
			++columnElasticNum)
		{
			for (UnsignedType indexMultip = 0; indexMultip < elasticRows;
				++indexMultip)
			{
				result[rowBtNum][columnElasticNum] +=
					bTranspose[rowBtNum][indexMultip] * 
					elasticConstMatrix[indexMultip][columnElasticNum];
			}
		}
	}
	return result;
}

// This function creates a diagonal mass matrix.
RealMatrix makeMatrixMassDiag(const UnsignedType& size,
	const FiniteElement& finiteElement)
{
	RealMatrix matrix(size, size);
	UnsignedType nNodes = size / 3;
	const Real dencity = finiteElement.dencity;
	const Real length = finiteElement.length;
	const Real width = finiteElement.width;
	const Real heigth = finiteElement.heigth;
	Real mass = dencity * length * width * heigth;

	if (nNodes == 0)
	{
		std::string msg = messageDivideZero();
		ERROR(msg);
	}

	for (UnsignedType indexDiagonal = 0; indexDiagonal < size; ++indexDiagonal)
	{
		matrix[indexDiagonal][indexDiagonal] = mass / static_cast<Real>(nNodes);
	}
	return matrix;
}

// This function creates a joint mass matrix.
RealMatrix makeMatrixMassJoint(const UnsignedType& size,
	const FiniteElement& finiteElement)
{
	const Real length = finiteElement.length;
	const Real width = finiteElement.width;
	const Real heigth = finiteElement.heigth;
	const Real detMatrixJacobian = length * width * heigth / 8.0;

	RealMatrix locCoord = getLocalCoordinate();
	const UnsignedType locCoordRows = locCoord.sizeRows();
	const UnsignedType locCoordColumns = locCoord.sizeColumns();

	const Real dencity = finiteElement.dencity;
	RealMatrix quadPoints = 
		makeMatrixQuadPoints(locCoordRows, locCoordColumns, locCoord);

	RealMatrix matrixMass(size, size);
	if (size < locCoordColumns)
	{
		std::string msg = messageOutOfRange();
		ERROR(msg);
	}

	for (UnsignedType nodeI = 0; nodeI < locCoordColumns; ++nodeI)
	{
		for (UnsignedType nodeJ = 0; nodeJ < locCoordColumns; ++nodeJ)
		{
			for (UnsignedType integPoint = 0; integPoint < locCoordColumns;
				++integPoint)
			{
				// locCoord[0][i] - ksi, locCoord[1][i] - etta, 
				// locCoord[2][i] - psi
				Array3D locPoint
				{ locCoord[0][nodeI],
					locCoord[1][nodeI],
					locCoord[2][nodeI] };

				// quadPoints[0][k] - quadratic point ksi, 
				// quadPoints[1][k] - quadratic point etta, 
				// quadPoints[2][k] - quadratic point psi
				Array3D quadPoint
				{ quadPoints[0][integPoint],
					quadPoints[1][integPoint],
					quadPoints[2][integPoint] };

				Real shapeFuncI = shapeFunction(locPoint, quadPoint);
				Real shapeFuncJ = shapeFunction(locPoint, quadPoint);

				ASSERT(3 * nodeI < size && 3 * nodeJ < size, msg);
				ASSERT((3 * nodeI + 1) < size && (3 * nodeJ + 1) < size, msg);
				ASSERT((3 * nodeI + 2) < size && (3 * nodeJ + 2) < size, msg);

				matrixMass[3 * nodeI][3 * nodeJ] += 
					shapeFuncI * shapeFuncJ;

				matrixMass[3 * nodeI + 1][3 * nodeJ + 1] +=
					shapeFuncI * shapeFuncJ;

				matrixMass[3 * nodeI + 2][3 * nodeJ + 2] +=
					shapeFuncI * shapeFuncJ;
			}

			matrixMass[3 * nodeI][3 * nodeJ] *=
				detMatrixJacobian * dencity;

			matrixMass[3 * nodeI + 1][3 * nodeJ + 1] *=
				detMatrixJacobian * dencity;

			matrixMass[3 * nodeI + 2][3 * nodeJ + 2] *=
				detMatrixJacobian * dencity;
		}
	}
	return matrixMass;
}

// This function creates a matrix stiffness.
RealMatrix makeMatrixStiffness(const FiniteElement& finiteElement)
{
	const Real length = finiteElement.length;
	const Real width = finiteElement.width;
	const Real heigth = finiteElement.heigth;
	const Real detMatrixJacobian = length * width * heigth / 8.0;

	const Real modulusElastic = finiteElement.modulusElastic;
	const Real poissonRatio = finiteElement.poissonRatio;
	RealMatrix ElasticConst = makeMatrixElConst(6, 6, modulusElastic,
		poissonRatio);
	const UnsignedType rowsElastic = ElasticConst.sizeRows();

	RealMatrix locCoord = getLocalCoordinate();
	const UnsignedType rowsLocCord = locCoord.sizeRows();
	const UnsignedType columnsLocCoord = locCoord.sizeColumns();
	RealMatrix quadPoints = 
		makeMatrixQuadPoints(rowsLocCord, columnsLocCoord, locCoord);
	const UnsignedType columnsQuad = quadPoints.sizeColumns();

	const UnsignedType sizeStiffness = rowsLocCord * columnsQuad;
	RealMatrix result(sizeStiffness, sizeStiffness);
	for (UnsignedType integPoint = 0; integPoint < columnsQuad; integPoint++)
	{
		UnsignedType nodeLoc = 0;
		Array3D locPoint{ locCoord[0][nodeLoc], locCoord[1][nodeLoc],
					locCoord[2][nodeLoc] };

		// construction of the differentiation matrix
		RealMatrix matrixDifferentiation(rowsElastic, sizeStiffness);
		for (UnsignedType indexNotZero = 0; indexNotZero < sizeStiffness - 2;
			indexNotZero += 3)
		{
			locPoint = { locCoord[0][nodeLoc], locCoord[1][nodeLoc],
					locCoord[2][nodeLoc] };

			matrixDifferentiation[0][indexNotZero] = 
				dShapeFuncKsi(locPoint, quadPoints[1][integPoint],
				quadPoints[2][integPoint], length);

			matrixDifferentiation[1][indexNotZero + 1] = 
				dShapeFuncEtta(locPoint, quadPoints[0][integPoint],
				quadPoints[2][integPoint], width);

			matrixDifferentiation[2][indexNotZero + 2] = 
				dShapeFuncPsi(locPoint, quadPoints[0][integPoint],
				quadPoints[1][integPoint], heigth);
			nodeLoc++;
		}

		nodeLoc = 0;
		for (UnsignedType indexNotZero = 0; indexNotZero < sizeStiffness - 2;
			indexNotZero += 3)
		{
			locPoint = { locCoord[0][nodeLoc], locCoord[1][nodeLoc],
			locCoord[2][nodeLoc] };

			matrixDifferentiation[3][indexNotZero] = 
				dShapeFuncEtta(locPoint, quadPoints[0][integPoint],
				quadPoints[2][integPoint], width);

			matrixDifferentiation[3][indexNotZero + 1] = 
				dShapeFuncKsi(locPoint, quadPoints[1][integPoint],
				quadPoints[2][integPoint], length);
			nodeLoc++;
		}

		nodeLoc = 0;
		for (UnsignedType indexNotZero = 0; indexNotZero < sizeStiffness - 2;
			indexNotZero += 3)
		{
			locPoint = { locCoord[0][nodeLoc], locCoord[1][nodeLoc],
			locCoord[2][nodeLoc] };

			matrixDifferentiation[4][indexNotZero + 1] = 
				dShapeFuncPsi(locPoint, quadPoints[0][integPoint],
				quadPoints[1][integPoint], heigth);

			matrixDifferentiation[4][indexNotZero + 2] = 
				dShapeFuncEtta(locPoint, quadPoints[0][integPoint],
				quadPoints[2][integPoint], width);
			nodeLoc++;
		}

		nodeLoc = 0;
		for (UnsignedType indexNotZero = 0; indexNotZero < sizeStiffness - 2;
			indexNotZero += 3)
		{
			locPoint = { locCoord[0][nodeLoc], locCoord[1][nodeLoc],
			locCoord[2][nodeLoc] };

			matrixDifferentiation[5][indexNotZero] = 
				dShapeFuncPsi(locPoint, quadPoints[0][integPoint],
				quadPoints[1][integPoint], heigth);

			matrixDifferentiation[5][indexNotZero + 2] = 
				dShapeFuncKsi(locPoint, quadPoints[1][integPoint],
				quadPoints[2][integPoint], length);
			nodeLoc++;
		}

		RealMatrix matrixBt = transpose(matrixDifferentiation);
		RealMatrix matrixBtD = makeMatrixBtD(matrixDifferentiation, ElasticConst);

		// Obtaining the final stiffness matrix
		if (matrixBtD.sizeRows() < sizeStiffness &&
			matrixDifferentiation.sizeColumns() < sizeStiffness)
		{
			std::string msg = messageOutOfRange();
			ERROR(msg);
		}

		for (UnsignedType rowNum = 0; rowNum < sizeStiffness; ++rowNum)
		{
			for (UnsignedType columnNum = 0; columnNum < sizeStiffness;
				++columnNum)
			{
				Real product = 0;
				if (matrixBtD.sizeColumns() < rowsElastic &&
					matrixDifferentiation.sizeRows() < rowsElastic)
				{
					std::string msg = messageOutOfRange();
					ERROR(msg);
				}

				if (matrixBtD.sizeColumns() != matrixDifferentiation.sizeRows())
				{
					std::string msg =
						"Matrix size mismatch : cols(A) != rows(B). ";
					ERROR(msg);
				}

				for (UnsignedType indexMultip = 0; indexMultip < rowsElastic;
					++indexMultip)
				{
					product += matrixBtD[rowNum][indexMultip] *
						matrixDifferentiation[indexMultip][columnNum];
				}
				result[rowNum][columnNum] += product * detMatrixJacobian;
			}
		}
	}

	return result;
}
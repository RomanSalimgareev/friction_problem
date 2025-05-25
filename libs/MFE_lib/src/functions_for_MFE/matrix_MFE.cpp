#include <MFE_lib/functions_for_MFE/matrix_MFE.h>

#include "error_handling/error_handling.h"
#include "inputFunctions/inputFunctions.h"

using namespace MFE;

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
			inputValue(value);
		}
		break;
	}
	case PROPERTIES_POISSONS_RATIO:
	{
		while (value <= DBL_EPSILON || value >= LIM_POISSONS_RATIO)
		{
			std::cout << "Invalid finite element poission's ratio, "
				"input the value again: \n";
			inputValue(value);
		}
		break;
	}
	case PROPERTIES_DENCITY:
	{
		while (value < MIN_DENCITY || value > MAX_DENCITY)
		{
			std::cout << "Invalid finite element dencity, "
				"input the value again: \n";
			inputValue(value);
		}
		break;
	}
	case PROPERTIES_LENGTH:
	{
		while (value < MIN_SIZE_FINITE_ELEMENT)
		{
			std::cout << "Invalid finite element length, "
				"input the value again: \n";
			inputValue(value);
		}
		break;
	}
	case PROPERTIES_WIDTH:
	{
		while (value < MIN_SIZE_FINITE_ELEMENT)
		{
			std::cout << "Invalid finite element width, "
				"input the value again: \n";
			inputValue(value);
		}
		break;
	}
	case PROPERTIES_HEIGTH:
	{
		while (value < MIN_SIZE_FINITE_ELEMENT)
		{
			std::cout << "Invalid finite element heigth, "
				"input the value again: \n";
			inputValue(value);
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

// Entering material properties manually
void MFE::setMaterialProperties(FiniteElement& finiteElement)
{
	std::cout << "Input modulus elastic: " << "\n";
	inputValue(finiteElement.modulusElastic);
	checkIncorrectValue(PROPERTIES_MODULUS_ELASTIC,
		finiteElement.modulusElastic);

	std::cout << "Input coefficient Puasson: " << "\n";
	inputValue(finiteElement.poissonRatio);
	checkIncorrectValue(PROPERTIES_POISSONS_RATIO,
		finiteElement.poissonRatio);

	std::cout << "Input dencity: " << "\n";
	inputValue(finiteElement.dencity);
	checkIncorrectValue(PROPERTIES_DENCITY, finiteElement.dencity);

	std::cout << "Input length final element: " << "\n";
	inputValue(finiteElement.length);
	checkIncorrectValue(PROPERTIES_LENGTH, finiteElement.length);

	std::cout << "Input width final element: " << "\n";
	inputValue(finiteElement.width);
	checkIncorrectValue(PROPERTIES_WIDTH, finiteElement.width);

	std::cout << "Input heigth final element: " << "\n";
	inputValue(finiteElement.heigth);
	checkIncorrectValue(PROPERTIES_HEIGTH, finiteElement.heigth);
}

// The choice of setting material properties: by default or manually
void MFE::chooseMaterialProperties(FiniteElement& finiteElement)
{
	char choice = 'n';
	std::cout << "Do you want to enter the parameters manually? (y/n) \n";
	inputChoice(choice);
	if (choice == 'y' || choice == 'Y')
		setMaterialProperties(finiteElement);
	else
		std::cout << "The parameters are selected by default: \n" <<
		"modulus elastic = " << finiteElement.modulusElastic << "; \n" <<
		"poisson's ratio = " << finiteElement.poissonRatio << "; \n" <<
		"dencity = " << finiteElement.dencity << "; \n" <<
		"length = " << finiteElement.length << "; \n" <<
		"width = " << finiteElement.width << "; \n" <<
		"heigth = " << finiteElement.heigth << "; \n\n";
}

// The shape function
Real MFE::shapeFunction(const Array3D& locPoint, const Array3D& quadPoint)
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
Real MFE::dShapeFuncKsi(const Array3D& locPoint,
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
Real MFE::dShapeFuncEtta(const Array3D& locPoint,
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
Real MFE::dShapeFuncPsi(const Array3D& locPoint,
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
RealMatrix MFE::getLocalCoordinate()
{
	RealMatrix matrix = { {1.0 ,1.0 ,-1.0, -1.0, 1.0, 1.0, -1.0, -1.0 },
		{ -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0 },
		{ -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0 } };

	return matrix;
}

// Its function creates a elastic constants matrix
RealMatrix MFE::makeMatrixElConst(const UnsignedType& rows,
	const UnsignedType& columns, const Real& modulusElastic,
	const Real& poissonRatio)
{
	RealMatrix matrix(rows, columns);
	if (poissonRatio >= LIM_POISSONS_RATIO)
	{
		std::string msg = "The Poisson's ratio is greater than or "
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
RealMatrix MFE::makeMatrixQuadPoints(const UnsignedType& rows,
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
// Its matrix product "bt" and "matrix of elastic constants".
RealMatrix MFE::makeMatrixBtD(const RealMatrix& bTranspose,
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
RealMatrix MFE::makeMatrixMassDiag(const UnsignedType& size,
	const FiniteElement& finiteElement)
{
	RealMatrix matrix(size, size);
	UnsignedType nNodes = size;
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

// This function creates a matrix stiffness.
RealMatrix MFE::makeMatrixStiffness(const FiniteElement& finiteElement)
{
	const Real length = finiteElement.length;
	const Real width = finiteElement.width;
	const Real height = finiteElement.heigth;
	const Real detMatrixJacobian = length * width * height / 8.0;

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
				quadPoints[1][integPoint], height);
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
				quadPoints[1][integPoint], height);

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
				quadPoints[1][integPoint], height);

			matrixDifferentiation[5][indexNotZero + 2] = 
				dShapeFuncKsi(locPoint, quadPoints[1][integPoint],
				quadPoints[2][integPoint], length);
			nodeLoc++;
		}

		RealMatrix matrixBt = transpose(matrixDifferentiation);
		RealMatrix matrixBtD = makeMatrixBtD(matrixBt, ElasticConst);

		// Obtaining the final stiffness matrix
		if (matrixBtD.sizeRows() < sizeStiffness ||
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
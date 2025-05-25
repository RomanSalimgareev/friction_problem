// This file contains functions for constructing a stiffness matrix, a joint mass 
// matrix and a diagonal mass matrix, as well as the functions for constructing
// these matrices themselves.

#ifndef MATRIX_MFE_H
#define MATRIX_MFE_H

#include <array>
#include <iostream>

#include <MFE_lib/math_functions.h>

namespace MFE
{
	using Array3D = std::array<Real, 3>;

	constexpr Real MIN_MODULUS_ELASTIC = 41.0e9;
	constexpr Real LIM_POISSONS_RATIO = 0.5;
	constexpr Real MIN_DENCITY = 1740.0;
	constexpr Real MAX_DENCITY = 19200.0;
	constexpr Real MIN_SIZE_FINITE_ELEMENT = 1.0e-3;

	enum Properties
	{
		PROPERTIES_MODULUS_ELASTIC,
		PROPERTIES_POISSONS_RATIO,
		PROPERTIES_DENCITY,
		PROPERTIES_LENGTH,
		PROPERTIES_WIDTH,
		PROPERTIES_HEIGTH,
	};

	// Finite element structure, default structure fields:
	// modulus of elasticity = 7e10;
	// poisson's ratio. = 0.33;
	// dencity = 2700.0;
	// length = 0.5
	// width = 0.06;
	// heigth = 0.05;
	struct MFE_LIB_EXPORT FiniteElement
	{
		// Material properties: modulus Elastic, Poisson's ratio, dencity
		Real modulusElastic = 7e10;
		Real poissonRatio = 0.33;
		Real dencity = 2700.0;

		// Dimensions of the final element
		Real length = 0.10;
		Real width = 0.06;
		Real heigth = 0.05;
	};

	// Entering material properties manually
	MFE_LIB_EXPORT void setMaterialProperties(FiniteElement& finiteElement);

	// The choice of setting material properties: by default or manually
	MFE_LIB_EXPORT void chooseMaterialProperties(FiniteElement& finiteElement);

	// This function gives a matrix of local coordinates for an 8 - 
	// node parallelepiped, where 1 row is the ksi coordinates,
	// 2 row is the etta coordinates, and 3 row is the psi coordinates.
	MFE_LIB_EXPORT RealMatrix getLocalCoordinate();

	// locCoord - these are the local coordinates of the point (ksi, etta, psi)

	// The shape function
	// quadPoint - these are the quadratic coordinates of a point, which are 
	// expressed in terms of local coordinates. 
	// example : quadratic coordinate first = sqrt(3.0) / 3.0 * ksi. 
	MFE_LIB_EXPORT Real shapeFunction(const Array3D& locCoord, const Array3D& quadPoint);

	// locCoord - these are the local coordinates of the point (ksi, etta, psi)

	// The derivative of the shape function by ksi
	MFE_LIB_EXPORT Real dShapeFuncKsi(const Array3D& locCoord,
		const Real& quadraticPointEtta, const Real& quadraticPointPsi,
		const Real& length);

	// The derivative of the shape function by etta
	MFE_LIB_EXPORT Real dShapeFuncEtta(const Array3D& locCoord,
		const Real& quadraticPointKsi, const Real& quadraticPointPsi,
		const Real& width);

	// The derivative of the shape function by psi
	MFE_LIB_EXPORT Real dShapeFuncPsi(const Array3D& locCoord,
		const Real& quadraticPointKsi, const Real& quadraticPointEtta,
		const Real& higth);

	// Its function creates a elastic constants matrix
	MFE_LIB_EXPORT RealMatrix makeMatrixElConst(const UnsignedType& rows,
		const UnsignedType& columns, const Real& modulusElastic,
		const Real& poissonRatio);

	// Its function creates a matrix of quadratic points
	MFE_LIB_EXPORT RealMatrix makeMatrixQuadPoints(const UnsignedType& rows,
		const UnsignedType& columns, const RealMatrix& localCoordinate);

	// Its function creates a matrix that is the product of the transposed
	// differentiation matrix and the matrix of elastic constants
	// There "bt" - the transposed matrix b, where matrix b is the
	// matrix of differentiation from the finite element method;
	// D is the matrix of elastic constants, i.e. the matrix ElasticConst.
	// Its matrix product "bt" and "matrix of elastic constants".
	MFE_LIB_EXPORT RealMatrix makeMatrixBtD(const RealMatrix& bTranspose,
		const RealMatrix& elasticConstMatrix);

	// Its function creates a diagonal mass matrix
	// size - this is the size of the stiffness matrix
	MFE_LIB_EXPORT RealMatrix makeMatrixMassDiag(const UnsignedType& size,
		const FiniteElement& finiteElement);

	// Its function creates a stiffness matrix
	MFE_LIB_EXPORT RealMatrix makeMatrixStiffness(const FiniteElement& finiteElement);

}

#endif
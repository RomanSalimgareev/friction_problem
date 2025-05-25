// This file is used in the solver_utils files.
// This file contains functions for setting kinematic initial conditions : 
// initial displacements, initial velocities, && initial accelerations.

#ifndef INITIAL_PARAMETERS_H
#define INITIAL_PARAMETERS_H

#include <iostream>
#include <array>

#include <MFE_lib/math_functions.h>

namespace MFE
{
	// It is not recommended to change to zero value.
	constexpr Real STATIC_SIMMETRY = 10.0e10;
	// It is not recommended to change the size of the array to zero
	constexpr std::array<UnsignedType, 4> INDICES_INITIAL = { 0, 3, 12, 15 };
	constexpr std::array<UnsignedType, 12> INDICES_SYMMETRY_CONDITION =
	{ 1, 2, 5, 6, 8, 9, 10, 11, 13, 18, 21, 22 };

	// Oscillation time setting function
	MFE_LIB_EXPORT Real inputTime();

	// Time step setting function
	MFE_LIB_EXPORT Real inputDeltaT();

	// Everywhere "size" is the row size of the stiffness matrix.

	// The function of setting a static load to solve a static problem.
	// initialForce - the force that you set to the edge
	MFE_LIB_EXPORT RealVector makeInitialStaticForce
	(const Real& initialForce, const UnsignedType& size);

	// The function of setting initial displacements by finding displacements from
	// a static problem or manually setting displacements
	MFE_LIB_EXPORT RealVector makeInitialDisps(RealMatrix matrixStiffness);

	// The function of setting the initial speeds manually.
	MFE_LIB_EXPORT RealVector makeInitialSpeed(const UnsignedType& size);

	// The function of setting the initial acceleration manually.
	MFE_LIB_EXPORT RealVector makeInitialAccel(const UnsignedType& size);

	// Applying symmetry conditions for a static problem on matrix stiffness.
	MFE_LIB_EXPORT void boundConditionStatic(RealMatrix& matrixStiffness);

	// The function of calculating the movements of a static problem
	// force - the static force
	MFE_LIB_EXPORT RealVector calculateDispStatic
	(const RealMatrix& matrixStiffness, const RealVector& force);

}

#endif

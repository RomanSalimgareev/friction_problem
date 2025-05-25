// This file contains functions for calculating the dynamic problem in the
// following cases : dry friction without a driving force,
// dry friction with a driving force, && viscous friction with a driving force.
// As well as the function of setting symmetry conditions.

#ifndef DINAMIC_SOLVER_MFE_H
#define DINAMIC_SOLVER_MFE_H

#include <algorithm>

#include <MFE_lib/functions_for_MFE/solver_utils.h>

namespace MFE
{
	// Coefficients that are present in the displacement equations.
	// These values ensure high accuracy and stability of the solution when using
	// the Nmork method.
	constexpr Real ALPHA = 0.25;
	constexpr Real DELTA = 0.5;

	// Applying symmetry conditions for a dinamic problem to a stiffness matrix,
	// a mass matrix, a velocity vector, a displacement vector
	// (meaning a row of the displacement matrix, since the matrix is constructed
	// from displacement vectors for each time step), an acceleration vector,
	// and a force vector.
	MFE_LIB_EXPORT void boundConditionsDinamic(RealMatrix& matrixStiffness,
		RealMatrix& matrixMass, RealMatrix& displacement,
		RealVector& speed, RealVector& acceleration,
		RealVector& force);

	// The decision functions take as input the number of time steps, the time step,
	// the stiffness matrix, the mass matrix, and the displacement matrix, in which
	// the initial displacements are recorded and in which the displacements will
	// be recorded.

	// stepsCount = time / deltaT

	// A function that solves the problem of dry friction without forcing force.
	// The function asks you to enter the coefficients of dry friction of rest
	// and dry friction of sliding.
	MFE_LIB_EXPORT void dryFrictionFree(const UnsignedType& stepsCount,
		const Real& deltaT, RealMatrix matrixStiffness,
		RealMatrix matrixMass, RealMatrix& displacements);

	// A function that solves the problem of dry friction with a driving force.
	// The function asks you to enter the coefficients of dry friction of rest
	// and dry friction of sliding.
	MFE_LIB_EXPORT void forcedDryFriction(const UnsignedType& stepsCount,
		const Real& deltaT, RealMatrix matrixStiffness,
		RealMatrix matrixMass, RealMatrix& displacements);

	// A function that solves the problem of viscous friction with 
	// a driving force.
	// The function asks you to enter the coefficient of viscous friction
	MFE_LIB_EXPORT void viscousFrictionForce(const UnsignedType& stepsCount,
		const Real& deltaT, RealMatrix matrixStiffness,
		RealMatrix matrixMass, RealMatrix& displacements);


	// Solver for dinamic problem.
	// It consists of a solver for the problem of dry friction without
	// a driving force, dry friction with a driving force and viscous
	// friction with a driving force.
	// The solver takes the oscillation time, time step, stiffness matrix
	// and mass matrix as input
	MFE_LIB_EXPORT RealMatrix calculateDisplacementsDinamic
	(const Real& time, const Real& deltaT,
		RealMatrix matrixStiffness, RealMatrix matrixMass);

}
#endif
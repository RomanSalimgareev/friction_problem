// This file contains functions for calculating the dynamic problem in the
// following cases : dry friction without a driving force,
// dry friction with a driving force, and viscous friction with a driving force.
// As well as the function of setting symmetry conditions.

#ifndef DINAMIC_SOLVER_MFE_H
#define DINAMIC_SOLVER_MFE_H

#include <algorithm>

#include "solver_utils.h"

// Coefficients that are present in the displacement equations.
// These values ensure good accuracy of the solution.
constexpr Real ALPHA = 0.25;
constexpr Real DELTA = 0.5;

// Applying symmetry conditions for a dinamic problem to a stiffness matrix,
// a mass matrix, a velocity vector, a displacement vector
// (meaning a row of the displacement matrix, since the matrix is constructed
// from displacement vectors for each time step), an acceleration vector,
// and a force vector.
void boundConditionsDinamic(RealMatrix& matrixStiffness,
	RealMatrix& matrixMass, RealMatrix& displacement,
	RealVector& speed, RealVector& acceleration,
	RealVector& force);

// stepsCount = time / deltaT

// A function that solves the problem of dry friction without forcing force.
// The function asks you to enter the coefficients of dry friction of rest
// and dry friction of sliding.
void dryFrictionFree(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass,
	RealMatrix& displacements);

// A function that solves the problem of dry friction with a driving force.
// The function asks you to enter the coefficients of dry friction of rest
// and dry friction of sliding.
void forcedDryFriction(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass,
	RealMatrix& displacements);

// A function that solves the problem of viscous friction with 
// a driving force.
// The function asks you to enter the coefficient of viscous friction
void viscousFrictionForce(const UnsignedType& stepsCount,
	const Real& deltaT, RealMatrix& matrixStiffness,
	RealMatrix& matrixMass, RealMatrix& displacements);


// Solver for dinamic problem.
// It consists of a solver for the problem of dry friction without
// a driving force, dry friction with a driving force and viscous
// friction with a driving force.
RealMatrix calculateDisplacementsDinamic
(const Real& time, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass);

#endif
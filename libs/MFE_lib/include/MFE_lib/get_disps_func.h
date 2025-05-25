// The file contains functions for obtaining node movements (active degrees of freedom).
// P.S.: Active degrees of freedom are the degrees of freedom through which movement occurs.

#ifndef GET_DISPS_FUNC_H
#define GET_DISPS_FUNC_H

#include <iostream>

#include <MFE_lib/types/matrix_type.h>
#include <MFE_lib/active_indices.h>

namespace MFE
{
	// Functions for obtaining displacements after solving from the 
	// displacement matrix

	// Getting the displacements of nodes 1, 2, 5, 6 in the direction of 
	// movement in a time step deltaT during time "time".
	// rows is the number of rows of the displacement matrix, and displacements is
	// the displacement matrix.
	MFE_LIB_EXPORT RealMatrix getDispAllNodes(const RealMatrix& displacements);

	// Getting the displacements of node 1 in the direction of movement 
	// in a time step deltaT during time "time".
	MFE_LIB_EXPORT RealVector getDispFirstNode(const RealMatrix& displacements);

	// Getting the displacements of node 2 in the direction of movement 
	// in a time step deltaT during time "time".
	MFE_LIB_EXPORT RealVector getDispSecondNode(const RealMatrix& displacements);

	// Getting the displacements of node 5 in the direction of movement 
	// in a time step deltaT during time "time".
	MFE_LIB_EXPORT RealVector getDispFifthNode(const RealMatrix& displacements);

	// Getting the displacements of node 6 in the direction of movement 
	// in a time step deltaT during time "time".
	MFE_LIB_EXPORT RealVector getDispSixthNode(const RealMatrix& displacements);

	// Getting a vector of time steps.
	// steps = time / deltaT
	MFE_LIB_EXPORT RealVector getStepsTime(const UnsignedType& steps, const Real& deltaT);

}

#endif

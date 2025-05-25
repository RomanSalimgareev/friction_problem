// This file contains functions for writing to a file "displacements.txt" moving
// nodes in the direction of movement, as well as the function of writing time
// steps to a file "time.txt ".

#ifndef WRITE_H
#define WRITE_H

#include <iostream>
#include <fstream>
#include <array>

#include <MFE_lib/types/matrix_type.h>
#include <MFE_lib/active_indices.h>

namespace MFE
{
	// If it is not written to the file, the functions return false, otherwise true.
	
	// Writing to the file the displacements of nodes 1, 2, 5, 6 in the direction of 
	// movement in a time step deltaT during time "time".
	// rows is the number of rows of the displacement matrix, and displacements is
	// the displacement matrix.
	MFE_LIB_EXPORT bool writeDispAllNodes(const RealMatrix& displacements);

	// Writing to the file the displacements of node 1 in the direction of 
	// movement in a time step deltaT during time "time".
	MFE_LIB_EXPORT bool writeDispFirstNode(const RealMatrix& displacements);

	// Writing to the file the displacements of node 2 in the direction of 
	// movement in a time step deltaT during time "time".
	MFE_LIB_EXPORT bool writeDispSecondNode(const RealMatrix& displacements);

	// Writing to the file the displacements of node 5 in the direction of 
	// movement in a time step deltaT during time "time".
	MFE_LIB_EXPORT bool writeDispFifthNode(const RealMatrix& displacements);

	// Writing to the file the displacements of node 6 in the direction of 
	// movement in a time step deltaT during time "time".
	MFE_LIB_EXPORT bool writeDispSixthNode(const RealMatrix& displacements);

	// Writing deltaT time intervals to a file during the "time" time.
	// steps = time / deltaT
	MFE_LIB_EXPORT bool writeStepsTime(const UnsignedType& steps, const Real& deltaT);

}

#endif

// This file contains functions for writing to a file "displacements.txt" moving
// nodes in the direction of movement, as well as the function of writing time
// steps to a file "stepsTime.txt ".

#ifndef WRITE_H
#define WRITE_H

#include <iostream>
#include <fstream>

#include "class_Matrix.h"

using Real = double;
using UnsignedType = UnsignedType;

// Writing to the file the movements of nodes 1, 2, 5, 6 in the direction of 
// movement in a time step deltaT during time time.
// rows is the number of rows of the displacement matrix, and displacements is
// the displacement matrix.
void writeDispAllNodes(const UnsignedType& rows,
	const Matrix<Real>& displacements);

// Writing to the file the movements of node 1 in the direction of movement 
// in a time step deltaT during time time.
void writeDispFirstNode(const UnsignedType& rows,
	const Matrix<Real>& displacements);

// Writing to the file the movements of node 2 in the direction of movement 
// in a time step deltaT during time time.
void writeDispSecondNode(const UnsignedType& rows,
	const Matrix<Real>& displacements);

// Writing to the file the movements of node 5 in the direction of movement 
// in a time step deltaT during time time.
void writeDispFifthNode(const UnsignedType& rows,
	const Matrix<Real>& displacements);

// Writing to the file the movements of node 6 in the direction of movement 
// in a time step deltaT during time time.
void writeDispSixthNode(const UnsignedType& rows,
	const Matrix<Real>& displacements);

// Writing deltaT time steps to the file during the time period.
// steps = time / deltaT
void writeStepsTime(const UnsignedType& steps, const Real& deltaT);

#endif

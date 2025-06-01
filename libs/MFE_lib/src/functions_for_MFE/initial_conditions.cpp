#include <MFE_lib/functions_for_MFE/initial_conditions.h>

#include "error_handling/error_handling.h"
#include "inputFunctions/inputFunctions.h"

using namespace MFE;

// Oscillation time setting function
Real MFE::inputTime()
{
	Real time = 0.0;
	std::cout << "Enter the oscillation time: \n";
	inputValue(time);

	return time;
}

// Time step setting function
Real MFE::inputDeltaT()
{
	Real deltaT = 0.0;
	std::cout << "Enter the time step \n" 
			  << "(It is recommended to take the accuracy of 1e-7 or 1e-8): \n";
	inputValue(deltaT);

	return deltaT;
}

// Setting the vector of initial static forces by active degrees of freedom
// to set the initial displacements
RealVector MFE::makeInitialStaticForce(const Real& initialForce,
	const UnsignedType& size)
{
	RealVector vectorInitial(size, 0.0);
	// Indices of the action of static forces 
	UnsignedType sizeIndices = INDICES_INITIAL.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the INDICES_INITIAL is 0. ";
		ERROR(msg);
	}

	Real nodeLoad = initialForce / static_cast<Real> (sizeIndices);
	for (auto index : INDICES_INITIAL)
	{
		if (index < size)
			vectorInitial[index] = nodeLoad;
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
	}

	return vectorInitial;
}

// Setting the initial displacements
RealVector MFE::makeInitialDisps
(RealMatrix matrixStiffness)
{
	const UnsignedType size = matrixStiffness.sizeRows();
	char choice = 'n';
	std::cout << "Do you want to set an initial displacements from " <<
		"a static task (y / n)? \n";
	inputChoice(choice);

	RealVector vectorInitial(size, 0.0);
	if (choice == 'y' || choice == 'Y')
	{

		boundConditionStatic(matrixStiffness);

		Real force = 0.0;
		std::cout << "Input the initial static load acting on the edge: \n";
		inputValue(force);

		if (force != 0.0)
		{
			RealVector initialForceStatic =
				makeInitialStaticForce(force, size);
			// Solution for static problem
			vectorInitial =
				calculateDispStatic(matrixStiffness, initialForceStatic);
		}
	}
	else
		std::cout << "Zero initial conditions are set" << "\n\n";

	return vectorInitial;
}

RealVector MFE::makeInitialSpeed(const UnsignedType& size)
{
	RealVector vectorInitial(size, 0.0);
	Real speed = 0.0;
	std::cout << "Input the initial speed: \n";
	inputValue(speed);

	if (speed != 0.0)
	{
		for (auto index : INDICES_INITIAL)
		{
			if (index < size)
				vectorInitial[index] = speed;
			else
			{
				std::string msg = messageOutOfRange();
				ASSERT(index < sizeForce, msg);
				WARNING(msg);
				continue;
			}
		}
	}

	return vectorInitial;
}

RealVector MFE::makeInitialAccel(const UnsignedType& size)
{
	RealVector vectorInitial(size, 0.0);
	Real acceleration = 0.0;
	std::cout << "Input the initial acceleration: \n";
	inputValue(acceleration);

	if (acceleration != 0.0)
	{
		for (auto index : INDICES_INITIAL)
		{
			if (index < size)
				vectorInitial[index] = acceleration;
			else
			{
				std::string msg = messageOutOfRange();
				ASSERT(index < sizeForce, msg);
				WARNING(msg);
				continue;
			}
		}
	}

	return vectorInitial;
}

// Applying symmetry conditions to symmetry nodes for a static problem
void MFE::boundConditionStatic(RealMatrix& matrixStiffness)
{
	for (const auto& index : INDICES_SYMMETRY_CONDITION)
	{
		bool isOutOfRange = index >= matrixStiffness.sizeRows() ||
			index >= matrixStiffness.sizeColumns();
		if (isOutOfRange)
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
		else
			matrixStiffness[index][index] *= STATIC_SIMMETRY;
	}
}

// Solver for a static problem
RealVector MFE::calculateDispStatic
(const RealMatrix& matrixStiffness, const RealVector& force)
{
	const UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealVector interimSolution(rowsStiffness, 0.0);
	RealMatrix matrixCholesky = createMatrixCholesky(matrixStiffness);
	if (force.size() < rowsStiffness)
	{
		std::string msg = "Vector size < matrix rows. ";
		ERROR(msg);
	}

	for (UnsignedType rowNum = 0; rowNum < rowsStiffness; ++rowNum)
	{
		Real sum = 0.0;
		for (UnsignedType columnNum = 0; columnNum < rowNum; ++columnNum)
		{
			sum += matrixCholesky[rowNum][columnNum] * 
				interimSolution[columnNum];
		}

		if (abs(matrixCholesky[rowNum][rowNum]) <= DBL_EPSILON)
		{
			std::string msg = messageDivideZero();
			ERROR(msg);
		}

		interimSolution[rowNum] = (force[rowNum] - sum) /
			matrixCholesky[rowNum][rowNum];
	}

	RealVector displacements(rowsStiffness, 0.0);

	UnsignedType rowsDisps = rowsStiffness;
	while (rowsDisps > 0)
	{
		--rowsDisps;

		Real sum = 0.0;
		for (UnsignedType columnNum = rowsDisps + 1; columnNum < rowsStiffness;
			++columnNum)
		{
			sum += matrixCholesky[columnNum][rowsDisps] * 
				displacements[columnNum];
		}

		if (matrixCholesky[rowsDisps][rowsDisps] <= DBL_EPSILON)
		{
			std::string msg = messageDivideZero();
			ERROR(msg);
		}

		displacements[rowsDisps] = interimSolution[rowsDisps] - sum;
		displacements[rowsDisps] = displacements[rowsDisps] /
			matrixCholesky[rowsDisps][rowsDisps];
	}
	return displacements;
}
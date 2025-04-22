#include "initial_conditions.h"
#include "error_handling.h"

using namespace MFE;

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

RealVector MFE::makeInitialDisps
(RealMatrix matrixStiffness)
{
	const UnsignedType size = matrixStiffness.sizeRows();
	char choiceInitialDisp = 'n';
	std::cout << "Do you want to set an initial displacements from " <<
		"a static task (y / n)? \n";
	std::cin >> choiceInitialDisp;

	RealVector vectorInitial(size, 0.0);
	if (choiceInitialDisp == 'y' || choiceInitialDisp == 'Y')
	{

		boundConditionStatic(matrixStiffness);

		// For my task, the force is 10000
		Real force = 0.0;
		std::cout << "Input the initial static load acting on the edge: \n";
		std::cin >> force;

		if (force != 0.0)
		{
			RealVector initialForceStatic =
				makeInitialStaticForce(force, size);
			// Solution for static problem
			vectorInitial = 
				calculateDispStatic(matrixStiffness, initialForceStatic);
		}
	}

	return vectorInitial;
}

RealVector MFE::makeInitialSpeed(const UnsignedType& size)
{
	RealVector vectorInitial(size, 0.0);
	Real speed = 0.0;
	std::cout << "Input the initial speed: \n";
	std::cin >> speed;

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
	std::cin >> acceleration;

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

		if (matrixCholesky[rowNum][rowNum] <= DBL_EPSILON)
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
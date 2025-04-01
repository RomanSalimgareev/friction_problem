#include "initial_conditions.h"

RealVector makeInitialStaticForce(const Real& initialForce,
	const UnsignedType& size)
{
	RealVector vectorInitial(size, 0.0);
	// Indices of the action of static forces 
	UnsignedType sizeIndices = INDICES_INITIAL.size();
	if (sizeIndices == 0)
		ERROR_SIZE_INDICES_INITIAL_ZERO();

	Real nodeLoad = initialForce / static_cast<Real> (sizeIndices);
	for (auto index : INDICES_INITIAL)
	{
		if (index < size)
			vectorInitial[index] = nodeLoad;
		else
		{
			ASSERT(index < size, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}

	return vectorInitial;
}

RealVector makeInitialDisps 
(RealMatrix matrixStiffness)
{
	const UnsignedType size = matrixStiffness.sizeRows();
	char choiceInitialDisp = 'n';
	std::cout << "Do you want to set an initial displacements from " <<
		"a static task (y / n)? \n";
	std::cin >> choiceInitialDisp;

	RealVector vectorInitial(size, 0.0);
	if (choiceInitialDisp == 'y' or choiceInitialDisp == 'Y')
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

RealVector makeInitialSpeed(const UnsignedType& size)
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
				ASSERT(index < size, "The index goes beyond the array. ");
				WARNING_INDEX_OUT_OF_RANGE();
				continue;
			}
		}
	}

	return vectorInitial;
}

RealVector makeInitialAccel(const UnsignedType& size)
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
				ASSERT(index < size, "The index goes beyond the array. ");
				WARNING_INDEX_OUT_OF_RANGE();
				continue;
			}
		}
	}

	return vectorInitial;
}

// Applying symmetry conditions to symmetry nodes for a static problem
void boundConditionStatic(RealMatrix& matrixStiffness)
{
	const std::vector<UnsignedType> indexs =
	{ 1, 2, 5, 6, 8, 9, 10, 11, 13, 18, 21, 22 };
	for (const auto& index : indexs)
	{
		bool isOutOfRange = index >= matrixStiffness.sizeRows() or
			index >= matrixStiffness.sizeColumns();
		if (isOutOfRange)
		{
			ASSERT(isOutOfRange, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
		else
			matrixStiffness[index][index] *= STATIC_SIMMETRY;
	}
}

// Solver for a static problem
RealVector calculateDispStatic
(const RealMatrix& matrixStiffness, const RealVector& force)
{
	const UnsignedType rows = matrixStiffness.sizeRows();
	RealVector interimSolution(rows, 0.0);
	RealMatrix matrixCholesky = createMatrixCholesky(matrixStiffness);
	if (force.size() < rows)
	{
		std::string msg = "The size of the force vector is smaller than \n"
			"the rows of the stiffness matrix. ";
		msg += std::string(__FILE__) + "\n";
		log(LogLevel::ERROR, msg);
		throw std::runtime_error(msg);
	}

	for (UnsignedType i = 0; i < rows; ++i)
	{
		Real sum = 0.0;
		for (UnsignedType j = 0; j < i; ++j)
		{
			sum += matrixCholesky[i][j] * interimSolution[j];
		}

		if (matrixCholesky[i][i] <= DBL_EPSILON)
			ERROR_DIVIDE_ZERO();

		interimSolution[i] = (force[i] - sum) / matrixCholesky[i][i];
	}

	RealVector displacements(rows, 0.0);

	UnsignedType i = rows;
	while (i > 0)
	{
		--i;

		Real sum = 0.0;
		for (UnsignedType j = i + 1; j < rows; ++j)
		{
			sum += matrixCholesky[j][i] * displacements[j];
		}

		if (matrixCholesky[i][i] <= DBL_EPSILON)
			ERROR_DIVIDE_ZERO();

		displacements[i] = (interimSolution[i] - sum) / matrixCholesky[i][i];
	}
	return displacements;
}
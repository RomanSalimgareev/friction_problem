#include "initial_conditions.h"

RealVector makeStaticForce(const Real& initialForce,
	const UnsignedType& size)
{
	RealVector vectorInitial(size, 0.0);
	// Øndices of the action of static forces 
	UnsignedType sizeIndices = INDICES_INITIAL.size();
	Real sizeCast = static_cast<Real> (sizeIndices);
	Real nodeLoad = initialForce / sizeCast;
	for (auto index : INDICES_INITIAL)
		vectorInitial[index] = nodeLoad;

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
				makeStaticForce(force, size);
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
		for (auto i : INDICES_INITIAL)
			vectorInitial[i] = speed;
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
		for (auto i : INDICES_INITIAL)
			vectorInitial[i] = acceleration;
	}

	return vectorInitial;
}

// Applying symmetry conditions to symmetry nodes for a static problem
void boundConditionStatic(RealMatrix& matrixStiffness)
{
	std::vector<UnsignedType> indexs =
	{ 1, 2, 5, 6, 8, 9, 10, 11, 13, 18, 21, 22 };
	for (auto i : indexs)
	{
		if (matrixStiffness[i][i] == 0.0)
			matrixStiffness[i][i] = STATIC_SIMMETRY;
		else
			matrixStiffness[i][i] *= STATIC_SIMMETRY;
	}
}

// Solver for a static problem
RealVector calculateDispStatic
(const RealMatrix& matrixStiffness, const RealVector& force)
{
	const UnsignedType rows = matrixStiffness.sizeRows();
	RealVector interimSolution(rows);
	RealMatrix matrixCholesky = createMatrixCholesky(matrixStiffness);
	for (UnsignedType i = 0; i < rows; ++i)
	{
		Real sum = 0.0;
		for (UnsignedType j = 0; j < i; ++j)
		{
			sum += matrixCholesky[i][j] * interimSolution[j];
		}
		interimSolution[i] = (force[i] - sum) / matrixCholesky[i][i];
	}

	RealVector displacements(rows);

	UnsignedType i = rows;
	while (i > 0)
	{
		--i;

		Real sum = 0.0;
		for (UnsignedType j = i + 1; j < rows; ++j)
		{
			sum += matrixCholesky[j][i] * displacements[j];
		}
		displacements[i] = (interimSolution[i] - sum) / matrixCholesky[i][i];
	}
	return displacements;
}
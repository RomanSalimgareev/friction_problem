#include "dinamic_solver_MFE.h"
#include "error_handling.h"
#include "operator_overloading.h"

using namespace MFE;

// Applying symmetry conditions to symmetry nodes for a dynamic problem
void MFE::boundConditionsDinamic(RealMatrix& matrixStiffness,
	RealMatrix& matrixMass, RealMatrix& displacements,
	RealVector& speed, RealVector& acceleration,
	RealVector& force)
{
	// Indices of degrees of freedom that are subject to symmetry conditions
	UnsignedType displacementRows = displacements.sizeRows();
	UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	if (rowsStiffness != matrixStiffness.sizeColumns())
	{
		std::string msg = "The stiffness matrix is not square. ";
		ERROR(msg);
	}

	bool isSizeEqual = rowsStiffness != matrixMass.sizeRows() &&
		matrixStiffness.sizeColumns() != matrixMass.sizeColumns();

	if (isSizeEqual == false)
	{
		std::string msg = "The sizes of the matrices are not equal. ";
		ERROR(msg);
	}

	// The corresponding columns && rows are deleted
	UnsignedType sizeIndicesSymmetry = INDICES_SYMMETRY_CONDITION.size();
	for (UnsignedType index = sizeIndicesSymmetry - 1; index > 0; --index)
	{
		UnsignedType rowNum = INDICES_SYMMETRY_CONDITION[index];

		matrixStiffness.eraseRow(rowNum);
		matrixMass.eraseRow(rowNum);

		if (rowNum < speed.size() && rowNum < acceleration.size() &&
			rowNum < force.size())
		{
			speed.erase(speed.begin() + rowNum);
			acceleration.erase(acceleration.begin() + rowNum);
			force.erase(force.begin() + rowNum);
		}
		else
		{
			std::string msg = "Invalid argument. ";
			ERROR(msg);
		}
	}

	for (UnsignedType index = sizeIndicesSymmetry - 1; index > 0; --index)
	{
		UnsignedType columnNum = INDICES_SYMMETRY_CONDITION[index];
		matrixStiffness.eraseColumn(columnNum);
		matrixMass.eraseColumn(columnNum);

		displacements.eraseColumn(columnNum);
	}
}

// A function that solves the problem of dry friction without forcing force.
void MFE::dryFrictionFree(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass,
	RealMatrix& displacements)
{
	Real coeffDryFrictionRest = 0.0;
	Real coeffDryFrictionSliding = 0.0;
	std::cout << "Input coefficient of dry friction at rest \n";
	std::cin >> coeffDryFrictionRest;
	std::cout << "Input coefficient of dry friction at sliding \n";
	std::cin >> coeffDryFrictionSliding;

	const UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealVector force(rowsStiffness);
	setForceNormReaction(force, NO_DRIVE_FORCE_DRY);

	RealVector speedOld = makeInitialSpeed(rowsStiffness);
	RealVector accelerationOld =
		makeInitialAccel(rowsStiffness);

	boundConditionsDinamic(matrixStiffness, matrixMass, displacements,
		speedOld, accelerationOld, force);

	RealVector speedNew(rowsStiffness, 0);
	RealVector accelerationNew(rowsStiffness, 0);
	Real averagePointsSpeedOld = 0.0;

	Real alphaDt2 = ALPHA * pow(deltaT, 2);
	Real alphaDt = ALPHA * deltaT;
	Real sumSteps = 0.0;
	for (UnsignedType step = 0; step < stepsCount - 1; ++step)
	{
		Real elasticForce =
			getElasticForce(displacements[step], matrixStiffness);

		Real averagePointsSpeed = getAveragePointsSpeed(speedOld);

		setForceDry(elasticForce, coeffDryFrictionRest,
			coeffDryFrictionSliding, averagePointsSpeed, force);

		// Check for sticking - projection of all forces on the KSI axis
		Real frictionForce = getSumFrictionForce(force);

		bool isFrictionGreater = abs(elasticForce) <= abs(frictionForce);
		bool isLowSpeed = abs(averagePointsSpeed) < EPS &&
			abs(averagePointsSpeedOld) < EPS;

		if (isFrictionGreater && isLowSpeed)
		{
			// The knot is sticking
			displacements[step + 1] = displacements[step];
		}
		else
		{
			// Calculation of displacements using formulas from the textbook,
			// as well as using the Gauss method with a leading element
			RealVector columnFreeMembers = alphaDt2 * force;
			columnFreeMembers += matrixMass * (displacements[step] +
				deltaT * (speedOld - (ALPHA - 0.5) * deltaT * accelerationOld));

			RealMatrix matrixCoefficients = 
				matrixMass + alphaDt2 * matrixStiffness;

			displacements[step + 1] = 
				solveGauss(matrixCoefficients, columnFreeMembers);

			RealVector differenceDisps =
				displacements[step + 1] - displacements[step];

			accelerationNew = 1.0 / alphaDt2 * differenceDisps;
			accelerationNew -= 1.0 / alphaDt * speedOld;
			accelerationNew += (1.0 - 0.5 / ALPHA) * accelerationOld;

			speedNew = (DELTA / alphaDt) * differenceDisps;
			speedNew += (1.0 - DELTA / ALPHA) * speedOld;
			speedNew += (1.0 - 0.5 * DELTA / ALPHA) * deltaT * accelerationOld;

			speedOld = speedNew;
			accelerationOld = accelerationNew;

		}
		averagePointsSpeedOld = averagePointsSpeed;
	}
}

// A function that solves the problem of dry friction with a driving force
void MFE::forcedDryFriction(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass,
	RealMatrix& displacements)
{
	Real coeffDryFrictionRest = 0.0;
	Real coeffDryFrictionSliding = 0.0;
	std::cout << "Input coefficient of dry friction at rest \n";
	std::cin >> coeffDryFrictionRest;
	std::cout << "Input coefficient of dry friction at sliding \n";
	std::cin >> coeffDryFrictionSliding;

	const UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealVector force(rowsStiffness, 0.0);
	setForceNormReaction(force, HAS_DRIVE_FORCE_DRY);

	RealVector speedOld = makeInitialSpeed(rowsStiffness);
	RealVector accelerationOld =
		makeInitialAccel(rowsStiffness);

	boundConditionsDinamic(matrixStiffness, matrixMass, displacements,
		speedOld, accelerationOld, force);

	Real averagePointsSpeedOld = 0.0;
	Real averagePointsSpeedNew = 0.0;

	Real frequency = getFrequencyForce(HAS_DRIVE_FORCE_DRY);

	Real amplitudeForce = getAmplitudeForce(DRIVE_FORCE_DRY);

	Real nodeLoad = getNodeLoad(amplitudeForce);

	RealVector speedNew(rowsStiffness, 0);
	RealVector accelerationNew(rowsStiffness, 0);

	Real alphaDt2 = ALPHA * pow(deltaT, 2);
	Real alphaDt = ALPHA * deltaT;
	Real sumSteps = 0.0;
	Real signForce = 0;
	for (UnsignedType step = 0; step < stepsCount - 1; ++step)
	{
		Real driveForceNode = nodeLoad * cos(frequency * sumSteps);
		Real elasticForce =
			getElasticForce(displacements[step], matrixStiffness);

		Real averagePointsSpeed = getAveragePointsSpeed(speedOld);

		signForce = getSignFrictionDrive(elasticForce, averagePointsSpeed,
			driveForceNode);

		Real coeffDryFriction = getCoeffDryFriction(coeffDryFrictionRest,
			coeffDryFrictionSliding, averagePointsSpeed);

		setForceDriveDry(signForce, coeffDryFrictionRest,
			coeffDryFrictionSliding, averagePointsSpeed,
			driveForceNode, force);

		// Check for sticking - projection of all forces on the KSI axis
		Real frictionForce = getSumFrictionForce(force);

		bool lowSpeedCondition = 
			isLowSpeedElement(averagePointsSpeedOld, averagePointsSpeed, 
				averagePointsSpeedNew);

		bool elasticForceCondition = isLowElasticForce(elasticForce,
			driveForceNode, frictionForce, signForce);

		bool driveForceCondition = isLowDriveForce(elasticForce,
			driveForceNode, frictionForce, signForce);


		bool driveElForceCondition = isLowDriveElastic(elasticForce,
			driveForceNode, frictionForce);

		bool driveElSignCondition = 
			isOneWayElasticDrive(elasticForce, driveForceNode, signForce);


		bool driveElasticCondition = driveElForceCondition &&
			driveElSignCondition;

		if (lowSpeedCondition &&
			(elasticForceCondition || driveForceCondition ||
				driveElasticCondition))
		{
			// The knot is sticking
			displacements[step + 1] = displacements[step];
		}
		else
		{
			// Calculation of displacements using formulas from the textbook,
			// as well as using the Gauss method with a leading element 

			RealVector accelTerm = (ALPHA - 0.5) * deltaT * accelerationOld;

			RealVector innerSum = displacements[step] +
				deltaT * (speedOld - accelTerm);

			RealVector freeMembers = alphaDt2 * force + matrixMass * innerSum;


			RealMatrix matrixCoeffs = matrixMass + alphaDt2 * matrixStiffness;

			displacements[step + 1] = solveGauss(matrixCoeffs, freeMembers);

			RealVector differenceDisps =
				displacements[step + 1] - displacements[step];

			if (ALPHA == 0 || deltaT == 0)
			{
				std::string msg = messageDivideZero();
				ERROR(msg);
			}

			accelerationNew = 1.0 / alphaDt2 * differenceDisps;
			accelerationNew -= 1.0 / alphaDt * speedOld;
			accelerationNew += ((1.0 - 1.0 / (2.0 * ALPHA)) * accelerationOld);

			speedNew = DELTA / alphaDt * differenceDisps;
			speedNew += (1.0 - DELTA / ALPHA) * speedOld;
			speedNew += (1.0 - DELTA / (2.0 * ALPHA)) * deltaT * accelerationOld;

			speedOld = speedNew;
			accelerationOld = accelerationNew;
		}

		averagePointsSpeedOld = averagePointsSpeedNew;
		averagePointsSpeedNew = averagePointsSpeed;
		sumSteps += deltaT;
	}
}

void MFE::viscousFrictionForce(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass,
	RealMatrix& displacements)
{
	Real coeffViscousfriction = 0.0;
	std::cout << "Input coefficient viscous friction:" << "\n";
	std::cin >> coeffViscousfriction;

	const UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealVector speedOld = makeInitialSpeed(rowsStiffness);
	RealVector accelerationOld =
		makeInitialAccel(rowsStiffness);

	RealVector force(rowsStiffness);
	boundConditionsDinamic(matrixStiffness, matrixMass, displacements,
		speedOld, accelerationOld, force);

	Real frequency = getFrequencyForce(DRIVE_FORCE_NO_DRY);

	Real amplitudeForce = getAmplitudeForce(DRIVE_FORCE_NO_DRY);

	Real nodeLoad = getNodeLoad(amplitudeForce);

	RealVector speedNew(rowsStiffness, 0);
	RealVector accelerationNew(rowsStiffness, 0);

	Real alphaDt2 = ALPHA * pow(deltaT, 2);
	Real alphaDt = ALPHA * deltaT;
	Real sumSteps = 0.0;

	// Calculation of displacements using formulas from the textbook,
	// as well as using the Gauss method with a leading element
	for (UnsignedType step = 0; step < stepsCount - 1; ++step)
	{
		setForceViscous(nodeLoad, frequency, sumSteps, force);

		RealVector dampingTerm = 
			DELTA * coeffViscousfriction * displacements[step];

		RealVector speedTerm = 
			speedOld * (1.0 + (DELTA - ALPHA) * deltaT * coeffViscousfriction);

		RealVector accelTerm = accelerationOld * ((ALPHA - 0.5) - 
			deltaT * (ALPHA - DELTA * 0.5) * coeffViscousfriction);

		RealVector innerSumFreeMembers = displacements[step] +
			deltaT * (dampingTerm + speedTerm - deltaT * accelTerm);

		RealVector freeMembers = alphaDt2 * force + 
			matrixMass * innerSumFreeMembers;
	

		RealMatrix innerSumMatrixCoeffs = alphaDt * matrixStiffness +
			DELTA * coeffViscousfriction * matrixMass;

		RealMatrix matrixCoeffs = matrixMass + deltaT * innerSumMatrixCoeffs;

		displacements[step + 1] = solveGauss(matrixCoeffs, freeMembers);

		RealVector differenceDisps =
			displacements[step + 1] - displacements[step];

		if (ALPHA == 0 || deltaT == 0)
		{
			std::string msg = messageDivideZero();
			ERROR(msg);
		}

		accelerationNew = 1.0 / alphaDt2 * differenceDisps;
		accelerationNew -= (1.0 / alphaDt) * speedOld;
		accelerationNew += (1.0 - 0.5 / ALPHA) * accelerationOld;

		speedNew = (DELTA / alphaDt) * differenceDisps;
		speedNew += (1.0 - DELTA / ALPHA) * speedOld;
		speedNew += (1.0 - 0.5 * DELTA / ALPHA) * deltaT * accelerationOld;

		speedOld = speedNew;
		accelerationOld = accelerationNew;
		sumSteps += deltaT;
	}
}

// Solver for a dinamic problem
RealMatrix MFE::calculateDisplacementsDinamic
(const Real& time, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass)
{
	RealVector initialDisplacements =
		makeInitialDisps(matrixStiffness);

	if (deltaT <= DBL_EPSILON)
	{
		std::string msg = messageDivideZero();
		ERROR(msg);
	}

	UnsignedType stepsCount = static_cast<UnsignedType> (time / deltaT);
	const UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealMatrix displacements(stepsCount, rowsStiffness);
	
	if (displacements.sizeRows() != 0)
		displacements[0] = initialDisplacements;
	else
		return RealMatrix(0, 0);

	UnsignedType choice = getFrictionMode();
	
	switch (choice)
	{
		case 1:
		{
			dryFrictionFree(stepsCount, deltaT, matrixStiffness, matrixMass,
				displacements);
			break;
		}
		case 2:
		{
			forcedDryFriction(stepsCount, deltaT, matrixStiffness, matrixMass,
				displacements);
			break;
		}
		case 3:
		{
			viscousFrictionForce(stepsCount, deltaT, matrixStiffness, matrixMass,
				displacements);
			break;
		}
	}

	return displacements;
}

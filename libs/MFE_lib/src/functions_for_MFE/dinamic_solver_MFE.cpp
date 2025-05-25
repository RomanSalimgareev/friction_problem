#include <MFE_lib/functions_for_MFE/dinamic_solver_MFE.h>

#include "operator_overloading/operator_overloading.h"
#include "error_handling/error_handling.h"
#include "inputFunctions/inputFunctions.h"

using namespace MFE;

// Checking that the vector of initial accelerations is not zero for
bool checkAccelInitialNoZero(const RealVector& accelerationOld)
{
	const UnsignedType size = accelerationOld.size();
	for (const auto& index : INDICES_INITIAL)
	{
		if (index < size)
		{
			if (accelerationOld[index] != 0.0)
				return true;
		}
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
	}
	return false;
}

// Applying symmetry conditions to symmetry nodes for a dynamic problem
void MFE::boundConditionsDinamic(RealMatrix& matrixStiffness,
	RealMatrix& matrixMass, RealMatrix& displacements,
	RealVector& speed, RealVector& acceleration,
	RealVector& force)
{
	// Indices of degrees of freedom that are subject to symmetry conditions
	UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	if (rowsStiffness != matrixStiffness.sizeColumns())
	{
		std::string msg = "The stiffness matrix is not square. ";
		ERROR(msg);
	}

	bool isSizeEqual = rowsStiffness == matrixMass.sizeRows() &&
		matrixStiffness.sizeColumns() == matrixMass.sizeColumns();

	if (!isSizeEqual)
	{
		std::string msg = "The sizes of the matrices are not equal. ";
		ERROR(msg);
	}

	// The corresponding columns and rows are deleted
	UnsignedType sizeIndicesSymmetry = INDICES_SYMMETRY_CONDITION.size();
	for (UnsignedType index = sizeIndicesSymmetry - 1; index-- > 0; )
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

	for (UnsignedType index = sizeIndicesSymmetry - 1; index-- > 0;)
	{
		UnsignedType columnNum = INDICES_SYMMETRY_CONDITION[index];
		matrixStiffness.eraseColumn(columnNum);
		matrixMass.eraseColumn(columnNum);
		displacements.eraseColumn(columnNum);
	}
}

// A function that solves the problem of dry friction without forcing force.
void MFE::dryFrictionFree(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix matrixStiffness, RealMatrix matrixMass,
	RealMatrix& displacements)
{
	Real coeffDryFrictionRest = 0.0;
	Real coeffDryFrictionSliding = 0.0;
	std::cout << "Input coefficient of dry friction at rest \n";
	inputValue(coeffDryFrictionRest);

	std::cout << "Input coefficient of dry friction at sliding \n";
	inputValue(coeffDryFrictionSliding);

	UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealVector force(rowsStiffness, 0.0);

	RealVector accelerationOld =
		makeInitialAccel(rowsStiffness);
	RealVector speedOld = makeInitialSpeed(rowsStiffness);

	bool accelInitialNoZero = checkAccelInitialNoZero(accelerationOld);

	boundConditionsDinamic(matrixStiffness, matrixMass, displacements,
		speedOld, accelerationOld, force);

	rowsStiffness = matrixStiffness.sizeRows();
	RealVector speedNew(rowsStiffness, 0);
	RealVector accelerationNew(rowsStiffness, 0);
	Real averagePointsSpeedOld = 0.0;

	UnsignedType forceSizeAfterConds = force.size();
	for (UnsignedType step = 0; step < stepsCount - 1; ++step)
	{
		setForceNormReaction(force, NO_DRIVE_FORCE_DRY);

		Real alphaDt2 = ALPHA * pow(deltaT, 2);
		Real alphaDt = ALPHA * deltaT;

		setForceElastic(displacements[step], matrixStiffness, force);

		Real elasticForceSum =
			getElasticForceSum(displacements[step], matrixStiffness);

		Real averagePointsSpeed = getAveragePointsSpeed(speedOld);

		Real signForce = getSignFrictionFree(elasticForceSum, averagePointsSpeed);

		Real frictionForceSum = 
			getSumFrictionForce(signForce, coeffDryFrictionRest,
			coeffDryFrictionSliding, averagePointsSpeed,
				NO_DRIVE_FORCE_DRY);

		setForceDry(frictionForceSum, force);

		// Check for sticking - projection of all forces on the KSI axis and average speed
		bool isFrictionGreater = abs(frictionForceSum) >= abs(elasticForceSum);
		bool isLowSpeed = abs(averagePointsSpeed) <= EPS &&
			abs(averagePointsSpeedOld) <= EPS;

		if (isFrictionGreater && isLowSpeed && !accelInitialNoZero)
		{
			// The knot is sticking
			displacements[step + 1] = displacements[step];
			speedOld = RealVector(rowsStiffness, 0.0);
			accelerationOld = RealVector(rowsStiffness, 0.0);
		}
		else
		{
			// Formulas are used according to the Newmor method and
			// finite differences.
			// Displacements are calculated using the reflection method.
			RealVector columnFreeMembers = alphaDt2 * force;
			columnFreeMembers += matrixMass * (displacements[step] +
				deltaT * (speedOld - (ALPHA - 0.5) * deltaT * accelerationOld));

			RealMatrix matrixCoefficients = 
				matrixMass + alphaDt2 * matrixStiffness;

			displacements[step + 1] = 
				solveReflectionMethod(matrixCoefficients, columnFreeMembers);

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
		accelInitialNoZero = false;
		averagePointsSpeedOld = averagePointsSpeed;
		force.clear();
		force.resize(forceSizeAfterConds, 0.0);
	}
}

// A function that solves the problem of dry friction with a driving force
void MFE::forcedDryFriction(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix matrixStiffness, RealMatrix matrixMass,
	RealMatrix& displacements)
{
	Real coeffDryFrictionRest = 0.0;
	Real coeffDryFrictionSliding = 0.0;
	std::cout << "Input coefficient of dry friction at rest \n";
	inputValue(coeffDryFrictionRest);

	std::cout << "Input coefficient of dry friction at sliding \n";
	inputValue(coeffDryFrictionSliding);

	UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealVector force(rowsStiffness, 0.0);

	RealVector accelerationOld =
		makeInitialAccel(rowsStiffness);
	RealVector speedOld = makeInitialSpeed(rowsStiffness);

	bool accelInitialNoZero = checkAccelInitialNoZero(accelerationOld);

	boundConditionsDinamic(matrixStiffness, matrixMass, displacements,
		speedOld, accelerationOld, force);

	rowsStiffness = matrixStiffness.sizeRows();
	Real averagePointsSpeedOld = 0.0;

	Real frequency = getFrequencyForce(HAS_DRIVE_FORCE_DRY);

	Real amplitudeForce = getAmplitudeForce(DRIVE_FORCE_DRY);

	Real nodeLoad = getNodeLoad(amplitudeForce);
	RealVector speedNew(rowsStiffness, 0);
	RealVector accelerationNew(rowsStiffness, 0);

	UnsignedType forceSizeAfterConds = force.size();
	Real sumSteps = 0.0;
	Real signForce = 0;
	for (UnsignedType step = 0; step < stepsCount - 1; ++step)
	{
		setForceNormReaction(force, HAS_DRIVE_FORCE_DRY);

		Real alphaDt2 = ALPHA * pow(deltaT, 2);
		Real alphaDt = ALPHA * deltaT;

		Real driveForceNode = nodeLoad * cos(frequency * sumSteps);
		Real elasticForceSum =
			getElasticForceSum(displacements[step], matrixStiffness);

		setForceElastic(displacements[step], matrixStiffness, force);

		Real averagePointsSpeed = getAveragePointsSpeed(speedOld);

		signForce = getSignFrictionDrive(elasticForceSum, averagePointsSpeed,
			driveForceNode);

		// Check for sticking - projection of all forces on the KSI axis
		Real frictionForceSum =
			getSumFrictionForce(signForce, coeffDryFrictionRest,
			coeffDryFrictionSliding, averagePointsSpeed,
				HAS_DRIVE_FORCE_DRY);

		setForceDriveDry(frictionForceSum,
			driveForceNode, force);

		bool lowSpeedCondition = 
			isLowSpeedElement(averagePointsSpeedOld, averagePointsSpeed);

		bool elasticForceCondition = isLowElasticForce(elasticForceSum,
			driveForceNode, frictionForceSum, signForce);

		bool driveForceCondition = isLowDriveForce(elasticForceSum,
			driveForceNode, frictionForceSum, signForce);

		bool driveElForceCondition = isLowDriveElastic(elasticForceSum,
			driveForceNode, frictionForceSum); 

		bool driveElSignCondition = 
			isOneWayElasticDrive(elasticForceSum, driveForceNode);

		bool driveElasticCondition = driveElForceCondition &&
			driveElSignCondition;

		if (lowSpeedCondition &&
			(elasticForceCondition || driveForceCondition ||
				driveElasticCondition) && !accelInitialNoZero)
		{
			// The knot is sticking
			displacements[step + 1] = displacements[step];
			speedNew = RealVector(rowsStiffness, 0.0);
			accelerationNew = RealVector(rowsStiffness, 0.0);
		}
		else
		{
			// Formulas are used according to the Newmor method and
			// finite differences.
			// Displacements are calculated using the reflection method.
			RealVector accelTerm = (ALPHA - 0.5) * deltaT * accelerationOld;

			RealVector innerSum = displacements[step] +
				deltaT * (speedOld - accelTerm);

			RealVector freeMembers = alphaDt2 * force + matrixMass * innerSum;


			RealMatrix matrixCoeffs = matrixMass + alphaDt2 * matrixStiffness;

			displacements[step + 1] = solveReflectionMethod(matrixCoeffs, freeMembers);
			RealVector differenceDisps =
				displacements[step + 1] - displacements[step];

			if (ALPHA == 0 || deltaT == 0)
			{
				std::string msg = messageDivideZero();
				ERROR(msg);
			}

			accelerationNew = 1.0 / alphaDt2 * differenceDisps;
			accelerationNew -= 1.0 / alphaDt * speedOld;
			accelerationNew += ((1.0 - 0.5 / ALPHA) * accelerationOld);

			speedNew = (DELTA / alphaDt) * differenceDisps;
			speedNew += (1.0 - DELTA / ALPHA) * speedOld;
			speedNew += (1.0 - (DELTA * 0.5) /  ALPHA) * deltaT * accelerationOld;

		}

		accelInitialNoZero = false;
		speedOld = speedNew;
		accelerationOld = accelerationNew;
		averagePointsSpeedOld = averagePointsSpeed;
		sumSteps += deltaT;
		force.clear();
		force.resize(forceSizeAfterConds, 0.0);
	}
}

void MFE::viscousFrictionForce(const UnsignedType& stepsCount, const Real& deltaT,
	RealMatrix matrixStiffness, RealMatrix matrixMass,
	RealMatrix& displacements)
{
	Real coeffViscousfriction = 0.0;
	std::cout << "Input coefficient viscous friction:" << "\n";
	inputValue(coeffViscousfriction);

	const UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealVector accelerationOld =
		makeInitialAccel(rowsStiffness);
	RealVector speedOld = makeInitialSpeed(rowsStiffness);

	RealVector force(rowsStiffness, 0.0);
	boundConditionsDinamic(matrixStiffness, matrixMass, displacements,
		speedOld, accelerationOld, force);

	Real frequency = getFrequencyForce(DRIVE_FORCE_NO_DRY);

	Real amplitudeForce = getAmplitudeForce(DRIVE_FORCE_NO_DRY);

	Real nodeLoad = getNodeLoad(amplitudeForce);

	RealVector speedNew(rowsStiffness, 0);
	RealVector accelerationNew(rowsStiffness, 0);

	UnsignedType forceSizeAfterConds = force.size();
	Real sumSteps = 0.0;

	// Formulas are used according to the Newmor method and
	// finite differences.
	// Displacements are calculated using the reflection method.
	for (UnsignedType step = 0; step < stepsCount - 1; ++step)
	{
		Real alphaDt2 = ALPHA * pow(deltaT, 2);
		Real alphaDt = ALPHA * deltaT;

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

		displacements[step + 1] = solveReflectionMethod(matrixCoeffs, freeMembers);

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
		speedNew += (1.0 - (0.5 * DELTA) / ALPHA) * deltaT * accelerationOld;

		speedOld = speedNew;
		accelerationOld = accelerationNew;
		sumSteps += deltaT;

		force.clear();
		force.resize(forceSizeAfterConds, 0.0);
	}
}

// Solver for a dinamic problem
RealMatrix MFE::calculateDisplacementsDinamic
(const Real& time, const Real& deltaT,
	RealMatrix matrixStiffness, RealMatrix matrixMass)
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

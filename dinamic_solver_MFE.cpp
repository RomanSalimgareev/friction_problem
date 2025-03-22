#include "dinamic_solver_MFE.h"

// Applying symmetry conditions to symmetry nodes for a dynamic problem
void boundConditionsDinamic(RealMatrix& matrixStiffness,
	RealMatrix& matrixMass, RealMatrix& displacement,
	RealVector& speed, RealVector& acceleration,
	RealVector& force)
{
	// Indices of degrees of freedom that are subject to symmetry conditions
	std::vector<UnsignedType> indexsConditions =
	{ 1, 2, 5, 6, 8, 9, 10 ,11, 13, 18, 21, 22 };
	UnsignedType displacementRows = displacement.sizeRows();
	UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	UnsignedType j = 0;

	// The corresponding columns and rows are deleted
	for (UnsignedType i = 0; i < rowsStiffness; ++i)
	{
		auto iter = std::find(indexsConditions.begin(),
			indexsConditions.end(), i);
		if (iter != indexsConditions.end())
		{
			matrixStiffness.eraseRowMatrix(j);
			matrixMass.eraseRowMatrix(j);
			speed.erase(speed.begin() + j);
			acceleration.erase(acceleration.begin() + j);
			force.erase(force.begin() + j);
		}
		else
			++j;
	}

	rowsStiffness = matrixStiffness.sizeRows();
	for (UnsignedType i = 0; i < rowsStiffness; ++i)
	{
		j = 0;
		UnsignedType k = 0;
		while (j != rowsStiffness)
		{
			auto iter = std::find(indexsConditions.begin(),
				indexsConditions.end(), k);
			if (iter != indexsConditions.end())
			{
				matrixStiffness[i].erase(matrixStiffness[i].begin() + j);
				matrixMass[i].erase(matrixMass[i].begin() + j);

				if (i < displacementRows)
					displacement[i].erase(displacement[i].begin() + j);
			}
			else
				j++;
			k++;
		}
	}
}

// A function that solves the problem of dry friction without forcing force.
void dryFrictionFree(const UnsignedType& stepsCount, const Real& deltaT,
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
	for (UnsignedType i = 0; i < stepsCount - 1; ++i)
	{
		Real elasticForce =
			getElasticForce(displacements[i], matrixStiffness);

		Real averagePointsSpeed = getAveragePointsSpeed(speedOld);

		setForceDry(elasticForce, coeffDryFrictionRest,
			coeffDryFrictionSliding, averagePointsSpeed, force);

		// Check for sticking - projection of all forces on the KSI axis
		Real frictionForce = getSumFrictionForce(force);

		bool isFrictionGreater = abs(elasticForce) <= abs(frictionForce);
		bool isLowSpeed = abs(averagePointsSpeed) < EPS and
			abs(averagePointsSpeedOld) < EPS;

		if (isFrictionGreater and isLowSpeed)
		{
			// The knot is sticking
			displacements[i + 1] = displacements[i];
		}
		else
		{
			// Calculation of displacements using formulas from the textbook,
			// as well as using the Gauss method with a leading element
			RealVector b = alphaDt2 * force +
				matrixMass * (displacements[i] +
					deltaT * (speedOld -
						(ALPHA - 0.5) * deltaT * accelerationOld));

			RealMatrix a = matrixMass + alphaDt2 * matrixStiffness;
			displacements[i + 1] = solveGauss(a, b);

			RealVector differenceDisps =
				displacements[i + 1] - displacements[i];

			accelerationNew = 1.0 / alphaDt2 * differenceDisps
				- (1.0 / alphaDt * speedOld)
				+ (1.0 - 0.5 / ALPHA) * accelerationOld;

			speedNew = (DELTA / alphaDt) * differenceDisps +
				(1.0 - DELTA / ALPHA) * speedOld +
				(1.0 - 0.5 * DELTA / ALPHA) * deltaT * accelerationOld;

			speedOld = speedNew;
			accelerationOld = accelerationNew;

		}
		averagePointsSpeedOld = averagePointsSpeed;
	}
}

// A function that solves the problem of dry friction with a driving force
void forcedDryFriction(const UnsignedType& stepsCount, const Real& deltaT,
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
	for (UnsignedType i = 0; i < stepsCount - 1; ++i)
	{
		Real driveForceNode = nodeLoad * cos(frequency * sumSteps);
		Real elasticForce =
			getElasticForce(displacements[i], matrixStiffness);

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


		bool driveElasticCondition = driveElForceCondition and
			driveElSignCondition;

		if (lowSpeedCondition and
			(elasticForceCondition or driveForceCondition or
				driveElasticCondition))
		{
			// The knot is sticking
			displacements[i + 1] = displacements[i];
		}
		else
		{
			// Calculation of displacements using formulas from the textbook,
			// as well as using the Gauss method with a leading element
			RealVector freeMembers = alphaDt2 * force +
				matrixMass * (displacements[i] + deltaT * (speedOld -
					(ALPHA - 0.5) * deltaT * accelerationOld));

			RealMatrix matrixCoeffs = matrixMass +
				alphaDt2 * matrixStiffness;
			displacements[i + 1] = solveGauss(matrixCoeffs, freeMembers);

			RealVector differenceDisps =
				displacements[i + 1] - displacements[i];

			accelerationNew = (1.0 / alphaDt2 * differenceDisps) -
				(1.0 / alphaDt * speedOld) +
				((1.0 - 1.0 / (2.0 * ALPHA)) * accelerationOld);

			speedNew = (DELTA / alphaDt * differenceDisps) +
				((1.0 - DELTA / ALPHA) * speedOld) +
				((1.0 - DELTA / (2.0 * ALPHA)) * deltaT * accelerationOld);

			speedOld = speedNew;
			accelerationOld = accelerationNew;
		}

		averagePointsSpeedOld = averagePointsSpeedNew;
		averagePointsSpeedNew = averagePointsSpeed;
		sumSteps += deltaT;
	}
}

void viscousFrictionForce(const UnsignedType& stepsCount, const Real& deltaT,
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
	for (UnsignedType i = 0; i < stepsCount - 1; ++i)
	{
		setForceViscous(nodeLoad, frequency, sumSteps, force);

		RealVector freeMembers = alphaDt2 * force +
			matrixMass * (displacements[i] +
				deltaT * (DELTA * coeffViscousfriction * displacements[i] +
					speedOld * (1.0 +
						(DELTA - ALPHA) * deltaT * coeffViscousfriction) -
					deltaT * accelerationOld * ((ALPHA - 0.5) -
						deltaT * (ALPHA - DELTA * 0.5) * coeffViscousfriction)));

		RealMatrix matrixCoeffs = matrixMass +
			deltaT * (alphaDt * matrixStiffness
				+ DELTA * coeffViscousfriction * matrixMass);

		displacements[i + 1] = solveGauss(matrixCoeffs, freeMembers);

		RealVector differenceDisps =
			displacements[i + 1] - displacements[i];

		accelerationNew = (1.0 / alphaDt2 * differenceDisps) -
			(1.0 / alphaDt) * speedOld +
			(1.0 - 0.5 / ALPHA) * accelerationOld;

		speedNew = (DELTA / alphaDt) * differenceDisps +
			(1.0 - DELTA / ALPHA) * speedOld +
			(1.0 - 0.5 * DELTA / ALPHA) * deltaT * accelerationOld;

		speedOld = speedNew;
		accelerationOld = accelerationNew;
		sumSteps += deltaT;
	}
}

// Solver for a dinamic problem
RealMatrix calculateDisplacementsDinamic
(const Real& time, const Real& deltaT,
	RealMatrix& matrixStiffness, RealMatrix& matrixMass)
{
	RealVector initialDisplacements =
		makeInitialDisps(matrixStiffness);

	UnsignedType stepsCount = static_cast<UnsignedType> (time / deltaT);
	const UnsignedType rowsStiffness = matrixStiffness.sizeRows();
	RealMatrix displacements(stepsCount, rowsStiffness);
	displacements[0] = initialDisplacements;

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

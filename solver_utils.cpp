#include "solver_utils.h"

Real getAmplitudeForce(const bool& isDriveForceDry)
{
	Real amplitudeForce = 0.0;
	if (isDriveForceDry)
	{
		amplitudeForce = 600.0;
		char choiceAmplitude = 'n';
		std::cout << "The default value of the amplitude of the \n" <<
			"driving force acting on the final element is 600, \n" <<
			"do you want to change it(y / n) ? \n";
		std::cin >> choiceAmplitude;
		if (choiceAmplitude == 'y' or choiceAmplitude == 'Y')
			std::cin >> amplitudeForce;
	}
	else
	{
		amplitudeForce = 220.0;
		char choiceAmplitude = 'n';
		std::cout << "The default value of the amplitude of the \n" <<
			"driving force acting on the final element is 220, " <<
			"do you want to change it (y / n) ? \n";
		std::cin >> choiceAmplitude;
		if (choiceAmplitude == 'y' or choiceAmplitude == 'Y')
			std::cin >> amplitudeForce;
	}

	return amplitudeForce;
}

Real getAveragePointsSpeed(RealVector speed)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
		ERROR_SIZE_ACTIVE_INDICES_ZERO();

	const UnsignedType sizeSpeed = speed.size();
	Real sumSpeed = 0.0;
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeSpeed)
			sumSpeed += speed[index];
		else
		{
			ASSERT(index < sizeSpeed, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}

	Real averagePointsSpeed = sumSpeed / static_cast<Real> (sizeIndices);

	return averagePointsSpeed;
}

Real getCoeffDryFriction(const Real& coeffDryFrictionRest,
	const Real& coeffDryFrictionSliding, const Real averagePointsSpeed)
{
	Real coeffDryFriction = 0.0;
	if (abs(averagePointsSpeed) < EPS)
		coeffDryFriction = coeffDryFrictionRest;
	else
		coeffDryFriction = coeffDryFrictionSliding;

	return coeffDryFriction;
}

Real getElasticForce(const RealVector& displacement,
	const RealMatrix& matrixStiffness)
{
	const UnsignedType qualityValues = matrixStiffness[0].size();
	RealVector sumVectorStiffness(qualityValues, 0.0);
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < qualityValues)
			sumVectorStiffness += matrixStiffness[index];
		else
		{
			ASSERT(index < qualityValues, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}

	Real elasticForce = -1.0 * displacement * sumVectorStiffness;

	return elasticForce;
}

Real getFrequencyForce(const bool& isDriveForceDry)
{
	Real frequency = 0.0;
	if (isDriveForceDry)
	{
		frequency = PI;
		char choiceFrequency = 'n';
		std::cout << "The default value of a driving force frequency is PI, \n" <<
			"do you want to change it (y/n)? \n";
		std::cin >> choiceFrequency;
		if (choiceFrequency == 'y' or choiceFrequency == 'Y')
		{
			Real coefficientFrequency = 0.0;
			std::cout << "Input the coefficient before the frequency " <<
				"of the driving force: \n";
			std::cin >> coefficientFrequency;
			frequency *= coefficientFrequency;
		}
	}
	else
	{
		frequency = 2.0 * PI;
		char choiceFrequency = 'n';
		std::cout << "The default value of a driving force frequency " <<
			"is 2 * PI, do you want to change it (y / n)? \n";
		std::cin >> choiceFrequency;
		if (choiceFrequency == 'y' or choiceFrequency == 'Y')
		{
			Real coefficientFrequency = 0.0;
			std::cout << "Input the coefficient before the frequency " <<
				"of the driving force: \n";
			std::cin >> coefficientFrequency;
			frequency *= coefficientFrequency;
		}
	}

	return frequency;
}

UnsignedType getFrictionMode()
{
	UnsignedType choice = 0;
	std::cout << "Input 1 or 2, or 3, where 1 is oscillations without \n"
		<< "a driving force, 2 is dry friction and 3 is viscous friction \n"
		<< "with driving force \n";

	while (choice != 1 and choice != 2 and choice != 3)
		std::cin >> choice;

	return choice;
}

Real getNodeLoad(const Real& amplitudeForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
		ERROR_SIZE_ACTIVE_INDICES_ZERO();

	return amplitudeForce / static_cast<Real>(sizeIndices);
}

Real getNormReaction(const bool& isDriveForce)
{
	Real normalReaction = 0.0;
	static bool isGetNormal = false;
	char choiceReaction = 'n';
	if (isDriveForce)
	{
		if (isGetNormal == false)
		{
			isGetNormal = true;
			std::cout << "The default value of a normal reaction is 100, " <<
				"do you want to change it (y/n)? \n";
			std::cin >> choiceReaction;
			if (choiceReaction == 'y' or choiceReaction == 'Y')
				std::cin >> normalReaction;
			else
				normalReaction = NORMAL_REACTION_DRIVE;
		}
		else
			normalReaction = NORMAL_REACTION_DRIVE;
	}
	else
	{
		if (isGetNormal == false)
		{
			isGetNormal = true;
			std::cout << "The default value of a normal reaction is 400, \n" <<
				"do you want to change it (y/n)? \n";
			std::cin >> choiceReaction;
			if (choiceReaction == 'y' or choiceReaction == 'Y')
				std::cin >> normalReaction;
			else
				normalReaction = NORMAL_REACTION_FREE;
		}
		else
			normalReaction = NORMAL_REACTION_FREE;
	}

	return normalReaction;
}

Real getSignFrictionDrive(const Real& elasticForce,
	const Real& averagePointsSpeed, const Real& driveForce)
{
	Real signForce = 0.0;
	if (averagePointsSpeed > EPS or
		(abs(averagePointsSpeed) < EPS and
			(4 * driveForce + elasticForce > 0.0)))
	{
		signForce = -1.0;
	}
	else if (averagePointsSpeed < -EPS or
		(abs(averagePointsSpeed) < EPS and
			(4 * driveForce + elasticForce < 0.0)))
	{
		signForce = 1.0;
	}

	return signForce;
}

Real getSignFrictionFree(const Real& elasticForce,
	const Real& averagePointsSpeed)
{
	Real signForce = 0.0;
	if (averagePointsSpeed > EPS or
		(abs(averagePointsSpeed) < EPS and elasticForce > 0.0))
		signForce = -1.0;
	else if (averagePointsSpeed < -EPS or
		(abs(averagePointsSpeed) < EPS and elasticForce < 0.0))
		signForce = 1.0;

	return signForce;
}

Real getSumFrictionForce(const RealVector& force)
{
	const UnsignedType sizeForce = force.size();
	Real sumForce = 0.0;
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeForce)
			sumForce += force[index];
		else
		{
			ASSERT(index < sizeForce, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}

	return sumForce;
}



void setForceNormReaction(RealVector& force,
	const bool& isDriveForce)
{
	Real normalReaction = getNormReaction(isDriveForce);

	std::vector<UnsignedType> indexsNormalReaction =
	{ 4, 7, 14, 16, 17, 19, 20, 23 };
	const UnsignedType sizeForce = force.size();
	for (auto index : indexsNormalReaction)
	{
		if (index < sizeForce)
			force[index] = -1.0 * normalReaction;
		else
		{
			ASSERT(index < sizeForce, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}
}

void setForceDry(const Real& elasticForce,
	const Real& coeffDryFrictionRest,
	const Real& coeffDryFrictionSliding, const Real averagePointsSpeed,
	RealVector& force)
{
	Real normalReaction = getNormReaction(NO_DRIVE_FORCE_DRY);
	Real signForce = getSignFrictionFree(elasticForce, averagePointsSpeed);
	Real coeffDryFriction =
		getCoeffDryFriction(coeffDryFrictionRest, coeffDryFrictionSliding,
			averagePointsSpeed);

	const UnsignedType sizeForce = force.size();
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeForce)
		{
			if (index == 1 or index == 4)
				force[index] = signForce * coeffDryFriction * normalReaction;
			else if (index == 6)
				force[index] = 2.0 * signForce * coeffDryFriction * normalReaction;
		}
		else
		{
			ASSERT(index < sizeForce, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}
}

void setForceDriveDry(const Real& signForce,
	const Real& coeffDryFrictionRest,
	const Real& coeffDryFrictionSliding, const Real averagePointsSpeed,
	const Real& driveForceNode, RealVector& force)
{
	Real normalReaction = getNormReaction(HAS_DRIVE_FORCE_DRY);
	Real coeffDryFriction =
		getCoeffDryFriction(coeffDryFrictionRest, coeffDryFrictionSliding,
			averagePointsSpeed);

	const UnsignedType sizeForce = force.size();
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeForce)
		{
			if (index == 0)
				force[index] = driveForceNode;
			else if (index == 1 or index == 4)
				force[index] = signForce * coeffDryFriction * normalReaction +
				driveForceNode;
			else
				force[index] = 2.0 * signForce * coeffDryFriction * normalReaction +
				driveForceNode;
		}
		else
		{
			ASSERT(index < sizeForce, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}
}

void setForceViscous(const Real& nodeLoad, const Real& frequency,
	const Real& sumSteps, RealVector& force)
{
	const UnsignedType sizeForce = force.size();
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeForce)
			force[index] = nodeLoad * cos(frequency * sumSteps);
		else
		{
			ASSERT(index < sizeForce, "The index goes beyond the array. ");
			WARNING_INDEX_OUT_OF_RANGE();
			continue;
		}
	}
}


bool isLowDriveElastic(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	Real sizeCast = static_cast<Real> (sizeIndices);
	return (abs(sizeCast * driveForceNode +
		elasticForce) - abs(frictionForce) < -EPS);
}

bool isLowDriveForce(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce,
	const Real& signForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	Real sizeCast = static_cast<Real> (sizeIndices);
	return (abs(sizeCast * driveForceNode) -
		abs(elasticForce + frictionForce)) < -EPS and
		elasticForce * signForce >= 0.0;
}

bool isLowElasticForce(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce,
	const Real& signForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	Real sizeCast = static_cast<Real> (sizeIndices);
	return (abs(elasticForce) -
		abs(sizeCast * driveForceNode + frictionForce) < -EPS) and
		driveForceNode * signForce >= 0.0;
}

bool isLowSpeedElement(const Real& averagePointsSpeedOld,
	const Real& averagePointsSpeed, const Real& averagePointsSpeedNew)
{
	return abs(averagePointsSpeed) < EPS and
		abs(averagePointsSpeedOld) < EPS and
		abs(averagePointsSpeedNew) < EPS;
}

bool isOneWayElasticDrive(const Real& elasticForce,
	const Real& driveForceNode, const Real& signForce)
{
	return driveForceNode * signForce <= 0.0 and
		signForce * elasticForce <= 0.0;
}
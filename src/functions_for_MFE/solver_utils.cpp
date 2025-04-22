#include "solver_utils.h"
#include "error_handling.h"
#include "operator_overloading.h"

using namespace MFE;

Real MFE::getAmplitudeForce(const bool& isDriveForceDry)
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
		if (choiceAmplitude == 'y' || choiceAmplitude == 'Y')
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
		if (choiceAmplitude == 'y' || choiceAmplitude == 'Y')
			std::cin >> amplitudeForce;
	}

	return amplitudeForce;
}

Real MFE::getAveragePointsSpeed(RealVector speed)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	const UnsignedType sizeSpeed = speed.size();
	Real sumSpeed = 0.0;
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeSpeed)
			sumSpeed += speed[index];
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeSpeed, msg);
			WARNING(msg);
			continue;
		}
	}

	Real averagePointsSpeed = sumSpeed / static_cast<Real> (sizeIndices);

	return averagePointsSpeed;
}

Real MFE::getCoeffDryFriction(const Real& coeffDryFrictionRest,
	const Real& coeffDryFrictionSliding, const Real averagePointsSpeed)
{
	Real coeffDryFriction = 0.0;
	if (abs(averagePointsSpeed) < EPS)
		coeffDryFriction = coeffDryFrictionRest;
	else
		coeffDryFriction = coeffDryFrictionSliding;

	return coeffDryFriction;
}

Real MFE::getElasticForce(const RealVector& displacement,
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
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeSpeed, msg);
			WARNING(msg);
			continue;
		}
	}

	Real elasticForce = -1.0 * displacement * sumVectorStiffness;

	return elasticForce;
}

Real MFE::getFrequencyForce(const bool& isDriveForceDry)
{
	Real frequency = 0.0;
	if (isDriveForceDry)
	{
		frequency = PI;
		char choiceFrequency = 'n';
		std::cout << "The default value of a driving force frequency is PI, \n" <<
			"do you want to change it (y/n)? \n";
		std::cin >> choiceFrequency;
		if (choiceFrequency == 'y' || choiceFrequency == 'Y')
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
		if (choiceFrequency == 'y' || choiceFrequency == 'Y')
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

UnsignedType MFE::getFrictionMode()
{
	UnsignedType choice = 0;
	std::cout << "Input 1 || 2, || 3, where 1 is oscillations without \n"
		<< "a driving force, 2 is dry friction && 3 is viscous friction \n"
		<< "with driving force \n";

	while (choice != 1 && choice != 2 && choice != 3)
		std::cin >> choice;

	return choice;
}

Real MFE::getNodeLoad(const Real& amplitudeForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INITIAL is 0. ";
		ERROR(msg);
	}

	return amplitudeForce / static_cast<Real>(sizeIndices);
}

Real MFE::getNormReaction(const bool& isDriveForce)
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
			if (choiceReaction == 'y' || choiceReaction == 'Y')
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
			if (choiceReaction == 'y' || choiceReaction == 'Y')
				std::cin >> normalReaction;
			else
				normalReaction = NORMAL_REACTION_FREE;
		}
		else
			normalReaction = NORMAL_REACTION_FREE;
	}

	return normalReaction;
}

Real MFE::getSignFrictionDrive(const Real& elasticForce,
	const Real& averagePointsSpeed, const Real& driveForce)
{
	Real signForce = 0.0;
	if (averagePointsSpeed > EPS ||
		(abs(averagePointsSpeed) < EPS &&
			(4 * driveForce + elasticForce > 0.0)))
	{
		signForce = -1.0;
	}
	else if (averagePointsSpeed < -EPS ||
		(abs(averagePointsSpeed) < EPS &&
			(4 * driveForce + elasticForce < 0.0)))
	{
		signForce = 1.0;
	}

	return signForce;
}

Real MFE::getSignFrictionFree(const Real& elasticForce,
	const Real& averagePointsSpeed)
{
	Real signForce = 0.0;
	if (averagePointsSpeed > EPS ||
		(abs(averagePointsSpeed) < EPS && elasticForce > 0.0))
		signForce = -1.0;
	else if (averagePointsSpeed < -EPS ||
		(abs(averagePointsSpeed) < EPS && elasticForce < 0.0))
		signForce = 1.0;

	return signForce;
}

Real MFE::getSumFrictionForce(const RealVector& force)
{
	const UnsignedType sizeForce = force.size();
	Real sumForce = 0.0;
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeForce)
			sumForce += force[index];
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
	}

	return sumForce;
}



void MFE::setForceNormReaction(RealVector& force,
	const bool& isDriveForce)
{
	Real normalReaction = getNormReaction(isDriveForce);

	const UnsignedType sizeForce = force.size();
	for (auto index : INDICES_NORMAL_REACTION)
	{
		if (index < sizeForce)
			force[index] = -1.0 * normalReaction;
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
	}
}

void MFE::setForceDry(const Real& elasticForce,
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
			if (index == 1 || index == 4)
				force[index] = signForce * coeffDryFriction * normalReaction;
			else if (index == 6)
				force[index] = 2.0 * signForce * coeffDryFriction * normalReaction;
		}
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
	}
}

void MFE::setForceDriveDry(const Real& signForce,
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
			else if (index == 1 || index == 4)
				force[index] = signForce * coeffDryFriction * normalReaction +
				driveForceNode;
			else
				force[index] = 2.0 * signForce * coeffDryFriction * normalReaction +
				driveForceNode;
		}
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
	}
}

void MFE::setForceViscous(const Real& nodeLoad, const Real& frequency,
	const Real& sumSteps, RealVector& force)
{
	const UnsignedType sizeForce = force.size();
	for (const auto& index : ACTIVE_INDICES)
	{
		if (index < sizeForce)
			force[index] = nodeLoad * cos(frequency * sumSteps);
		else
		{
			std::string msg = messageOutOfRange();
			ASSERT(index < sizeForce, msg);
			WARNING(msg);
			continue;
		}
	}
}


bool MFE::isLowDriveElastic(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	Real sizeCast = static_cast<Real> (sizeIndices);
	return (abs(sizeCast * driveForceNode +
		elasticForce) - abs(frictionForce) < -EPS);
}

bool MFE::isLowDriveForce(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce,
	const Real& signForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	Real sizeCast = static_cast<Real> (sizeIndices);
	return (abs(sizeCast * driveForceNode) -
		abs(elasticForce + frictionForce)) < -EPS &&
		elasticForce * signForce >= 0.0;
}

bool MFE::isLowElasticForce(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce,
	const Real& signForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	Real sizeCast = static_cast<Real> (sizeIndices);
	return (abs(elasticForce) -
		abs(sizeCast * driveForceNode + frictionForce) < -EPS) &&
		driveForceNode * signForce >= 0.0;
}

bool MFE::isLowSpeedElement(const Real& averagePointsSpeedOld,
	const Real& averagePointsSpeed, const Real& averagePointsSpeedNew)
{
	return abs(averagePointsSpeed) < EPS &&
		abs(averagePointsSpeedOld) < EPS &&
		abs(averagePointsSpeedNew) < EPS;
}

bool MFE::isOneWayElasticDrive(const Real& elasticForce,
	const Real& driveForceNode, const Real& signForce)
{
	return driveForceNode * signForce <= 0.0 &&
		signForce * elasticForce <= 0.0;
}
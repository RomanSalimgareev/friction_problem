#include <MFE_lib/functions_for_MFE/solver_utils.h>

#include "error_handling/error_handling.h"
#include "operator_overloading/operator_overloading.h"
#include "inputFunctions/inputFunctions.h"

using namespace MFE;

// The selection function for getAmplitudeForce
void choiceAmplitude(Real& amplitude)
{
	std::cout << "The default value of the amplitude of the \n"
		<< "driving force acting on the final element is "
		<< amplitude << ",\n" << "do you want to change it(y / n) ? \n";
	char choice = 'n';
	inputChoice(choice);

	bool isChoiceYes = choice == 'y' || choice == 'Y';
	if (isChoiceYes)
	{
		std::cout << "Input the amplitude value:" << "\n";
		inputValue(amplitude);
	}
	else
		std::cout << "The default value is : " << amplitude << "\n\n";
}

// Selection of the amplitude of the driving force manually or by default
Real MFE::getAmplitudeForce(const bool& isDriveForceDry)
{
	Real amplitudeForce = 0.0;
	if (isDriveForceDry)
	{
		amplitudeForce = 300.0;
		choiceAmplitude(amplitudeForce);
	}
	else
	{
		amplitudeForce = 220.0;
		choiceAmplitude(amplitudeForce);
	}

	return amplitudeForce;
}

// Getting the average speed based on active indices (motion indices)
Real MFE::getAveragePointsSpeed(RealVector speed)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	const UnsignedType sizeSpeed = speed.size();
	const UnsignedType lastIndexActive = ACTIVE_INDICES.back();
	Real sumSpeed = 0.0;
	if (sizeSpeed > lastIndexActive)
	{
		for (const auto& index : ACTIVE_INDICES)
			sumSpeed += speed[index];
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(sizeSpeed > lastIndexActive, msg);
		WARNING(msg);
	}

	Real averagePointsSpeed = sumSpeed / static_cast<Real> (sizeIndices);

	return averagePointsSpeed;
}

// Obtaining the coefficient of friction depending on the average speed
Real MFE::getCoeffDryFriction(const Real& coeffDryFrictionRest,
	const Real& coeffDryFrictionSliding, const Real averagePointsSpeed)
{
	if (abs(averagePointsSpeed) < EPS)
		return coeffDryFrictionRest;
	else
		return coeffDryFrictionSliding;

}

// Obtaining the total strength of elasticity according to active indices
Real MFE::getElasticForceSum(const RealVector& displacement,
	const RealMatrix& matrixStiffness)
{
	if (ACTIVE_INDICES.size() == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	UnsignedType qualityValues = 0;
	if (matrixStiffness.sizeRows() != 0)
		qualityValues = matrixStiffness.sizeRows();
	else
	{
		std::string msg = "The size of the matrixStiffness is 0. ";
		ERROR(msg);
	}

	const UnsignedType lastIndexActive = ACTIVE_INDICES.back();
	RealVector sumVectorStiffness(qualityValues, 0.0);

	if (qualityValues > lastIndexActive)
	{
		for (const auto& index : ACTIVE_INDICES)
			sumVectorStiffness += matrixStiffness[index];
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(qualityValues > lastIndexActive, msg);
		WARNING(msg);
	}

	// The usual Coulomb Law
	Real elasticForce = -1.0 * sumVectorStiffness * displacement;

	return elasticForce;
}

// The selection function for getFrequencyForce
void choiceFrequency(Real& frequencyCoeff)
{
	std::cout << "The default value of a driving force frequency is "
		<< frequencyCoeff << " * PI,\n" << "do you want to change it (y/n)? \n";
	char choice = 'n';
	inputChoice(choice);
	bool isChoiceYes = choice == 'y' || choice == 'Y';
	if (isChoiceYes)
	{
		std::cout << "Input the coefficient before the frequency " <<
			"of the driving force: \n";
		inputValue(frequencyCoeff);
	}
	else
		std::cout << "The default value is: " << frequencyCoeff << " * PI\n\n";
}

// The function of selecting the frequency of the driving force for
// getFrequencyForce, by setting the coefficient before PI
Real MFE::getFrequencyForce(const bool& isDriveForceDry)
{
	Real frequency = 0.0;
	Real frequencyCoeff = 0.0;
	if (isDriveForceDry)
	{
		frequencyCoeff = 40.0;
		frequency = frequencyCoeff * PI;
		choiceFrequency(frequencyCoeff);
	}
	else
	{
		frequencyCoeff = 2.0;
		frequency = frequencyCoeff * PI;
		choiceFrequency(frequencyCoeff);
	}

	return frequency;
}

// Problem selection
UnsignedType MFE::getFrictionMode()
{
	UnsignedType choice = 0;
	std::cout << "Enter 1 or 2, or 3, where 1 is a problem with dry friction \n"
		<< "without a driving force, 2 is a problem with dry friction with a \n"
		<< "driving force, and 3 is a problem with viscous friction and \n"
		<< "a driving force. \n";

	while (choice != 1 && choice != 2 && choice != 3)
		inputValue(choice);

	return choice;
}

// Obtaining the amplitude for the driving force in one node
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

// The selection function for getNormReaction
void choiceNormalReaction(bool& isGetNormal, Real& normalReaction)
{
	if (!isGetNormal)
	{
		isGetNormal = true;
		std::cout << "The default value of a normal reaction is "
				  << normalReaction << ", \n" 
				  << "do you want to change it (y/n)? \n";

		char choice = 'n';
		inputChoice(choice);
		bool isChoiceYes = choice == 'y' || choice == 'Y';
		if (isChoiceYes)
		{
			std::cout << "Input the value of the normal reaction: \n";
			inputValue(normalReaction);
		}
		else
			std::cout << "The default value is : " << normalReaction << "\n\n";
	}
}

// Getting a normal reaction to one degree of freedom (not total)
Real MFE::getNormReaction(const bool& isDriveForce)
{
	Real normalReaction = 0.0;
	static bool isGetNormal = false;
	if (isDriveForce)
	{
		normalReaction = NORMAL_REACTION_DRIVE;
		if (!isGetNormal)
			choiceNormalReaction(isGetNormal, normalReaction);
	}
	else
	{
		normalReaction = NORMAL_REACTION_FREE;
		if (!isGetNormal)
			choiceNormalReaction(isGetNormal, normalReaction);
	}

	return normalReaction;
}

// Obtaining the sign of the direction of the friction force
// for the problem of dry friction without a driving force
Real MFE::getSignFrictionDrive(const Real& elasticForce,
	const Real& averagePointsSpeed, const Real& driveForce)
{
	Real signForce = 0.0;
	if (averagePointsSpeed > EPS ||
		(abs(averagePointsSpeed) <= EPS &&
			(4 * driveForce + elasticForce > 0.0)))
	{
		signForce = -1.0;
	}
	else if (averagePointsSpeed < -EPS ||
		(abs(averagePointsSpeed) <= EPS &&
			(4 * driveForce + elasticForce < 0.0)))
	{
		signForce = 1.0;
	}

	return signForce;
}

// Obtaining the sign of the direction of the friction force
// for the problem of dry friction without a driving force
Real MFE::getSignFrictionFree(const Real& elasticForce,
	const Real& averagePointsSpeed)
{
	Real signForce = 0.0;
	if (averagePointsSpeed > EPS ||
		(abs(averagePointsSpeed) <= EPS && elasticForce > 0.0))
		signForce = -1.0;
	else if (averagePointsSpeed < -EPS ||
		(abs(averagePointsSpeed) <= EPS && elasticForce < 0.0))
		signForce = 1.0;

	return signForce;
}

// Obtaining the total friction force by active degrees of freedom
Real MFE::getSumFrictionForce(const Real& signForce,
	const Real& coeffDryFrictionRest, const Real& coeffDryFrictionSliding,
	const Real& averagePointsSpeed, const bool& isDriveForce)
{
	if (ACTIVE_INDICES.size() == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	Real sumForce = 0.0;
	Real coeffDryFriction = getCoeffDryFriction(coeffDryFrictionRest,
		coeffDryFrictionSliding, averagePointsSpeed);

	Real normalReaction = 0.0;
	if (isDriveForce)
		normalReaction = getNormReaction(HAS_DRIVE_FORCE_DRY);
	else
		normalReaction = getNormReaction(NO_DRIVE_FORCE_DRY);

	for (const auto& index : ACTIVE_INDICES)
	{
		if (index == 1 || index == 4)
			sumForce += signForce * coeffDryFriction * normalReaction;
		else if (index == 6)
			sumForce += 2 * signForce * coeffDryFriction * normalReaction;
	}

	return sumForce;
}

// The function of obtaining a stimulating force
Real MFE::getResultantForce(const RealVector& force)
{
	if (ACTIVE_INDICES.size() == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	Real resultantForce = 0.0;
	const UnsignedType lastIndexActive = ACTIVE_INDICES.back();
	const UnsignedType sizeForce = force.size();
	if (sizeForce > lastIndexActive)
	{
		for (const auto& index : ACTIVE_INDICES)
			resultantForce += force[index];
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(sizeForce > lastIndexActive, msg);
		WARNING(msg);
	}

	return resultantForce;
}

// Setting the elastic force by active degrees of freedom
void MFE::setForceElastic(const RealVector& displacement,
	const RealMatrix& matrixStiffness, RealVector& force)
{
	if (ACTIVE_INDICES.size() == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	UnsignedType qualityValues = 0;
	if (matrixStiffness.sizeRows() != 0)
		qualityValues = matrixStiffness.sizeRows();
	else
	{
		std::string msg = "The size of the matrixStiffness is 0. ";
		ERROR(msg);
	}

	const UnsignedType forceSize = force.size();
	const UnsignedType lastIndexActive = ACTIVE_INDICES.back();

	bool isIndicesCorrect = qualityValues > lastIndexActive &&
		forceSize > lastIndexActive;

	if (isIndicesCorrect)
	{
		for (const auto& index : ACTIVE_INDICES)
			force[index] += -1.0 * matrixStiffness[index] * displacement;
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(isIndicesCorrect > lastIndexActive, msg);
		WARNING(msg);
	}
}

// Setting the normal reaction force by degrees of freedom of wall pressure
void MFE::setForceNormReaction(RealVector& force,
	const bool& isDriveForce)
{
	if (INDICES_NORMAL_REACTION_ACTIVE.size() == 0)
	{
		std::string msg = "The size of the INDICES_NORMAL_REACTION_ACTIVE is 0.";
		ERROR(msg);
	}

	Real normalReaction = getNormReaction(isDriveForce);
	const UnsignedType sizeForce = force.size();
	const UnsignedType lastIndexNormal = INDICES_NORMAL_REACTION_ACTIVE.back();

	if (sizeForce > lastIndexNormal)
	{
		for (auto index : INDICES_NORMAL_REACTION_ACTIVE)
			force[index] += -1.0 * normalReaction;
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(sizeForce > lastIndexNormal, msg);
		WARNING(msg);
	}
}

// Setting the driving force for active degrees of freedom for the problem 
// of dry friction without a driving force
void MFE::setForceDry(const Real& frictionForceSum, RealVector& force)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	const UnsignedType sizeForce = force.size();
	const UnsignedType lastIndexActive = ACTIVE_INDICES.back();
	Real sizeCast = static_cast<Real>(sizeIndices);
	if (sizeForce > lastIndexActive)
	{
		for (const auto& index : ACTIVE_INDICES)
		{
			if (index == 1 || index == 4)
				force[index] += frictionForceSum / sizeCast;
			else if (index == 6)
				force[index] += 2.0 * frictionForceSum / sizeCast;
		}
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(sizeForce > lastIndexActive, msg);
		WARNING(msg);
	}
}

// Setting the driving force and friction for active degrees of freedom
// for the problem of dry friction with a driving force
void MFE::setForceDriveDry(const Real& forceFrictionSum,
	const Real& driveForceNode, RealVector& force)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	const UnsignedType sizeForce = force.size();
	const UnsignedType lastIndexActive = ACTIVE_INDICES.back();
	Real sizeCast = static_cast<Real>(sizeIndices);

	if (sizeForce > lastIndexActive)
	{
		for (const auto& index : ACTIVE_INDICES)
		{
			if (index == 0)
				force[index] += driveForceNode;
			else if (index == 1 || index == 4)
				force[index] += forceFrictionSum / sizeCast +
				driveForceNode;
			else
				force[index] += 2.0 * forceFrictionSum / sizeCast +
				driveForceNode;
		}
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(sizeForce > lastIndexActive, msg);
		WARNING(msg);
	}
}

// Setting the driving force for active degrees of freedom for the viscous
// friction problem
void MFE::setForceViscous(const Real& nodeLoad, const Real& frequency,
	const Real& sumSteps, RealVector& force)
{
	if (ACTIVE_INDICES.size() == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	const UnsignedType sizeForce = force.size();
	const UnsignedType lastIndexActive = ACTIVE_INDICES.back();

	if (sizeForce > lastIndexActive)
	{
		for (const auto& index : ACTIVE_INDICES)
			force[index] += nodeLoad * cos(frequency * sumSteps);
	}
	else
	{
		std::string msg = messageOutOfRange();
		ASSERT(sizeForce > lastIndexActive, msg);
		WARNING(msg);
	}
}


bool MFE::isLowDriveElastic(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	Real sizeCast = static_cast<Real> (sizeIndices);

	bool condiditionSign = abs(sizeCast * driveForceNode +
		elasticForce) - abs(frictionForce) < 0.0;
	bool conditionLess = abs(abs(sizeCast * driveForceNode +
		elasticForce) - abs(frictionForce)) <= EPS;

	return condiditionSign || conditionLess;
}

bool MFE::isLowDriveForce(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce,
	const Real& signForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	Real sizeCast = static_cast<Real> (sizeIndices);

	bool condiditionSign = abs(sizeCast * driveForceNode) -
		abs(elasticForce + frictionForce) < 0.0 && 
		elasticForce * signForce >= 0.0;
	bool conditionLess = abs(elasticForce) 
		< abs(sizeCast * driveForceNode);

	return condiditionSign && conditionLess;
}

bool MFE::isLowElasticForce(const Real& elasticForce,
	const Real& driveForceNode, const Real& frictionForce,
	const Real& signForce)
{
	UnsignedType sizeIndices = ACTIVE_INDICES.size();
	if (sizeIndices == 0)
	{
		std::string msg = "The size of the ACTIVE_INDICES is 0. ";
		ERROR(msg);
	}

	Real sizeCast = static_cast<Real> (sizeIndices);

	bool condiditionSign = abs(elasticForce) -
		abs(sizeCast * driveForceNode + frictionForce) < 0.0 &&
		driveForceNode * signForce >= 0.0;
	bool conditionLess = abs(sizeCast * driveForceNode) < 
		abs(elasticForce);

	return condiditionSign && conditionLess;
}
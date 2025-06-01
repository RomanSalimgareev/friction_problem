// This file is used in the dinamic_solver files.
// This file contains various get, set, && bool expression functions
// for checking conditions in dinamic_solver files.
#ifndef GET_SET_CONDIDION_H
#define GET_SET_CONDIDION_H

#include <MFE_lib/functions_for_MFE/initial_conditions.h>
#include <MFE_lib/active_indices.h>

namespace MFE
{
	constexpr Real PI = 3.14159265358979323846;
	constexpr Real NORMAL_REACTION_FREE = 400.0;
	constexpr Real NORMAL_REACTION_DRIVE = 100.0;
	constexpr Real EPS = 1.0e-4;
	constexpr bool HAS_DRIVE_FORCE_DRY = true;
	constexpr bool NO_DRIVE_FORCE_DRY = !HAS_DRIVE_FORCE_DRY;
	constexpr bool DRIVE_FORCE_DRY = true;
	constexpr bool DRIVE_FORCE_NO_DRY = !DRIVE_FORCE_DRY;
	constexpr std::array<UnsignedType, 8> INDICES_NORMAL_REACTION_ACTIVE =
	{ 2, 3, 5, 7, 8, 9, 10, 11 };

	// The function of obtaining the amplitude of the driving force for dry
	// friction and viscous.
	// For dry, the default is 300, for viscous 220
	MFE_LIB_EXPORT Real getAmplitudeForce(const bool& isDriveForceDry);

	// The function of obtaining the average velocity from the velocity vector
	// of the degrees of freedom of the nodes of interest
	MFE_LIB_EXPORT Real getAveragePointsSpeed(RealVector speed);

	// The function of obtaining the coefficient of friction depending on the
	// average speed: either the coefficient of friction at rest, if the speed
	// is less than eps, or the coefficient of friction sliding, if the speed
	// is greater than eps.
	MFE_LIB_EXPORT Real getCoeffDryFriction(const Real& coeffDryFrictionRest,
		const Real& coeffDryFrictionSliding,
		const Real averagePointsSpeed);

	// The function of obtaining the elastic force of the degrees of freedom of
	// the nodes of interest by displacement and stiffness matrix (analogous to
	// the stiffness of a spring).
	MFE_LIB_EXPORT Real getElasticForceSum(const RealVector& displacement,
		const RealMatrix& matrixStiffness);

	// The function of obtaining the frequency of the driving force depending on
	// friction is either dry or viscous.
	// For dry, the default is 40 * PI, for viscous 2 * PI
	MFE_LIB_EXPORT Real getFrequencyForce(const bool& isDriveForceDry);

	// The function choice of the solver depending on the friction: 1 - dry friction 
	// without a driving force; 2 - dry friction with a driving force;
	// 3 - viscous friction with a driving force.
	// Used for the calculateDisplacementsDinamic function to select a problem
	MFE_LIB_EXPORT UnsignedType getFrictionMode();

	// The amplitude distribution function across nodes 
	// (degrees of freedom in the direction of motion).
	MFE_LIB_EXPORT Real getNodeLoad(const Real& amplitudeForce);

	// The function of obtaining a normal reaction, depending on the problem,
	// is dry friction without forcing or dry friction with a forcing force.
	// For friction without a driving force, the normal wall response is
	// NORMAL_REACTION_FREE by default,
	// with a driving force NORMAL_REACTION_DRIVE
	MFE_LIB_EXPORT Real getNormReaction(const bool& isDriveForce);

	// The function returns the sign of the friction force in a problem with
	// a driving force depending on the average velocity or, if the velocity is
	// less than EPS, then on the direction of the resulting force projected on ksi.
	MFE_LIB_EXPORT Real getSignFrictionDrive(const Real& elasticForce,
		const Real& averagePointsSpeed, const Real& driveForce);

	// The function returns the sign of the friction force in a problem without
	// a driving force depending on the average velocity or, if the velocity is
	// less than EPS, then on the direction of the resulting force projected on ksi.
	MFE_LIB_EXPORT Real getSignFrictionFree(const Real& elasticForce,
		const Real& averagePointsSpeed);

	// The function returns the total dry friction force across the nodes.
	MFE_LIB_EXPORT Real getSumFrictionForce(const Real& signForce,
		const Real& coeffDryFrictionRest, const Real& coeffDryFrictionSliding,
		const Real& averagePointsSpeed, const bool& isDriveForce);
	
	// The function of obtaining a resultant force
	MFE_LIB_EXPORT Real getResultantForce(const RealVector& force);

	// Assignment to the vector of elastic forces
	MFE_LIB_EXPORT void setForceElastic(const RealVector& displacement,
		const RealMatrix& matrixStiffness, RealVector& force);

	// Setting the normal reaction at the nodes (corresponding to degrees of freedom)
	// affected by the normal reaction from the walls
	MFE_LIB_EXPORT void setForceNormReaction(RealVector& force,
		const bool& isDriveForce);

	// Setting the friction force at the nodes(degrees of freedom in the direction
	// of motion) for the problem of dry friction without a driving force
	MFE_LIB_EXPORT void setForceDry(const Real& frictionForceSum, RealVector& force);

	// Setting the driving force and the friction force at the nodes (degrees 
	// of freedom in the direction of motion) for the dry friction problem
	MFE_LIB_EXPORT void setForceDriveDry(const Real& forceFrictionSum,
		const Real& driveForceNode, RealVector& force);

	// Setting the driving force by nodes (degrees of freedom in the direction
	// of motion) for the viscous friction problem
	MFE_LIB_EXPORT void setForceViscous(const Real& nodeLoad, const Real& frequency,
		const Real& sumSteps, RealVector& force);


	// Checking that the sum of the elastic force and the driving force is less
	// than the friction force.
	// The driving force is co - directional with the elastic force,
	// and the friction force is opposite.
	MFE_LIB_EXPORT bool isLowDriveElastic(const Real& elasticForce,
		const Real& driveForceNode, const Real& frictionForce);

	// Checking that the driving force is less than the sum of the friction force
	// and the elastic force
	// The elastic force and the friction force are co-directional,
	// while the driving force is opposite.
	MFE_LIB_EXPORT bool isLowDriveForce(const Real& elasticForce,
		const Real& driveForceNode, const Real& frictionForce,
		const Real& signForce);

	// Checking that the elastic force is less than the sum of the friction
	// force and the forcing force.
	// The driving force and the friction force are co-directional,
	// while the elastic force is opposite.
	MFE_LIB_EXPORT bool isLowElasticForce(const Real& elasticForce,
		const Real& driveForceNode, const Real& frictionForce,
		const Real& signForce);

	// Checking that the speed of the element is less than EPS
	MFE_LIB_EXPORT inline bool isLowSpeedElement(const Real& averagePointsSpeedOld,
		const Real& averagePointsSpeed)
	{
		return abs(averagePointsSpeed) <= EPS && abs(averagePointsSpeedOld) <= EPS;
	}

	// Checking that the elastic force and the driving force are aligned
	MFE_LIB_EXPORT inline bool isOneWayElasticDrive(const Real& elasticForce,
		const Real& driveForceNode)
	{
		return driveForceNode * elasticForce >= 0.0;
	}

}

#endif
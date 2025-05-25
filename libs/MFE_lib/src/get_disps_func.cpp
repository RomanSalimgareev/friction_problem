#include <MFE_lib/get_disps_func.h>

#include "error_handling/error_handling.h"

using namespace MFE;

std::string messageNotReceived()
{
	return "The displacements are not received. ";
}

RealMatrix MFE::getDispAllNodes(const RealMatrix& displacements)
{
	const UnsignedType rows = displacements.sizeRows();
	const UnsignedType columns = displacements.sizeRows();
	RealMatrix allDisps(rows, columns);
	const UnsignedType lastActiveIndex = ACTIVE_INDICES.back();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= lastActiveIndex + 1)
		{
			for (const auto& index : ACTIVE_INDICES)
			allDisps[rowNum][index] = displacements[rowNum][index];
		}
		else
		{
			std::string msg = messageNotReceived();
			std::cout << msg << "\n";
			WARNING(msg);
		}
	}

	return allDisps;
}

RealVector MFE::getDispFirstNode(const RealMatrix& displacements)
{
	const UnsignedType rows = displacements.sizeRows();
	RealVector dispFirstNode(rows, 0.0);
	const UnsignedType& firstIndex = ACTIVE_INDICES.front();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= firstIndex + 1)
			dispFirstNode[rowNum] = displacements[rowNum][firstIndex];
		else
		{
			std::string msg = messageNotReceived();
			std::cout << msg << "\n";
			WARNING(msg);
		}
	}
	return dispFirstNode;
}

RealVector MFE::getDispSecondNode(const RealMatrix& displacements)
{
	const UnsignedType rows = displacements.sizeRows();
	RealVector dispSecondNode(rows, 0.0);
	const UnsignedType& secondIndex = ACTIVE_INDICES[1];
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= secondIndex + 1)
			dispSecondNode[rowNum] = displacements[rowNum][secondIndex];
		else
		{
			std::string msg = messageNotReceived();
			std::cout << msg << "\n";
			WARNING(msg);
		}
	}
	return dispSecondNode;
}

RealVector MFE::getDispFifthNode(const RealMatrix& displacements)
{
	const UnsignedType rows = displacements.sizeRows();
	RealVector dispFifthNode(rows, 0.0);
	const UnsignedType& third = ACTIVE_INDICES[3];
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= third + 1)
			dispFifthNode[rowNum] = displacements[rowNum][third];
		else
		{
			std::string msg = messageNotReceived();
			std::cout << msg << "\n";
			WARNING(msg);
		}
	}
	return dispFifthNode;
}

RealVector MFE::getDispSixthNode(const RealMatrix& displacements)
{
	const UnsignedType rows = displacements.sizeRows();
	RealVector dispSixthNode(rows, 0.0);
	const UnsignedType& fourthIndex = ACTIVE_INDICES.back();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= fourthIndex + 1)
			dispSixthNode[rowNum] = displacements[rowNum][fourthIndex];
		else
		{
			std::string msg = messageNotReceived();
			std::cout << msg << "\n";
			WARNING(msg);
		}
	}
	return dispSixthNode;
}

RealVector MFE::getStepsTime(const UnsignedType& steps, const Real& deltaT)
{
	RealVector stepsTime(steps, 0.0);
	for (UnsignedType step = 0; step < steps; ++step)
		stepsTime[step] = deltaT * step;

	return stepsTime;
}
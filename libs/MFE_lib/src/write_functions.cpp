#include <MFE_lib/write_functions.h>

#include "error_handling/error_handling.h"

using namespace MFE;

std::string messageNotRecorded()
{
	return "The displacements are not recorded. ";
}

void checkFileOpen(std::ofstream& fout, bool& isWrite)
{
	if (!fout.is_open())
	{
		std::string msg = "The file was not opened. ";
		WARNING(msg);

		isWrite = false;
	}
	std::cout << "File is open. \n";
}

bool MFE::writeDispAllNodes(const RealMatrix& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	const UnsignedType lastActiveIndex = ACTIVE_INDICES.back();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= lastActiveIndex + 1)
		{
			for (const auto& index : ACTIVE_INDICES)
			{
				if (index == lastActiveIndex)
					fout << std::to_string(index) << ": " 
						 << displacements[rowNum][index] << "\n";
				else
					fout << std::to_string(index) << ": "
					<< displacements[rowNum][index] << "  ";
			}
		}
		else
		{
			std::string msg = messageNotRecorded();
			std::cout << msg << "\n";
			WARNING(msg);

			isWrite = false;
		}
	}
	fout.close();

	return isWrite;
}

bool MFE::writeDispFirstNode(const RealMatrix& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	const UnsignedType& firstIndex = ACTIVE_INDICES.front();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= firstIndex + 1)
			fout << displacements[rowNum][firstIndex] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			std::cout << msg << "\n";
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();

	return isWrite;
}

bool MFE::writeDispSecondNode(const RealMatrix& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	const UnsignedType& secondIndex = ACTIVE_INDICES[1];
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= secondIndex + 1)
			fout << displacements[rowNum][secondIndex] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			std::cout << msg << "\n";
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();
	
	return isWrite;
}

bool MFE::writeDispFifthNode(const RealMatrix& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	const UnsignedType& thirdIndex = ACTIVE_INDICES[3];
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= thirdIndex + 1)
			fout << displacements[rowNum][thirdIndex] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			std::cout << msg << "\n";
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();

	return isWrite;
}

bool MFE::writeDispSixthNode(const RealMatrix& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	const UnsignedType& fourthIndex = ACTIVE_INDICES.back();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= fourthIndex + 1)
			fout << displacements[rowNum][fourthIndex] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			std::cout << msg << "\n";
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();

	return isWrite;
}

bool MFE::writeStepsTime(const UnsignedType& steps, const Real& deltaT)
{
	bool isWrite = true;
	std::ofstream fout("time.txt");
	checkFileOpen(fout, isWrite);

	for (UnsignedType step = 0; step < steps; ++step)
	{
		fout << deltaT * step << "\n";
	}
	fout.close();

	return isWrite;
}
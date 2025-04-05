#include "write_function.h"

std::string messageNotRecorded()
{
	return "The displacements are not recorded. ";
}

void checkFileOpen(std::ofstream& fout, bool& isWrite)
{
	if (fout.is_open() == false)
	{
		std::string msg = "The file was not opened. ";
		WARNING(msg);

		isWrite = false;
	}
	std::cout << "File is not open. \n";
}

bool writeDispAllNodes(const Matrix<Real>& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= 7)
		{
			fout << "1: " << displacements[rowNum][0] << "  ";
			fout << "2: " << displacements[rowNum][1] << "  ";
			fout << "5: " << displacements[rowNum][4] << "  ";
			fout << "6: " << displacements[rowNum][6] << "\n";
		}
		else
		{
			std::string msg = messageNotRecorded();
			WARNING(msg);

			isWrite = false;
			break;
		}
	}
	fout.close();

	return isWrite;
}

bool writeDispFirstNode(const Matrix<Real>& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= 1)
			fout << displacements[rowNum][0] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();

	return isWrite;
}

bool writeDispSecondNode(const Matrix<Real>& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= 2)
			fout << displacements[rowNum][1] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();
	
	return isWrite;
}

bool writeDispFifthNode(const Matrix<Real>& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= 5)
			fout << displacements[rowNum][4] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();

	return isWrite;
}

bool writeDispSixthNode(const Matrix<Real>& displacements)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	const UnsignedType rows = displacements.sizeRows();
	for (UnsignedType rowNum = 0; rowNum < rows; ++rowNum)
	{
		if (displacements[rowNum].size() >= 7)
			fout << displacements[rowNum][6] << "\n";
		else
		{
			std::string msg = messageNotRecorded();
			WARNING(msg);

			isWrite = false;
			break;
		}
	}

	fout.close();

	return isWrite;
}

bool writeStepsTime(const UnsignedType& steps, const Real& deltaT)
{
	bool isWrite = true;
	std::ofstream fout("displacements.txt");
	checkFileOpen(fout, isWrite);

	for (UnsignedType step = 0; step < steps; ++step)
	{
		fout << deltaT * step << "\n";
	}
	fout.close();

	return isWrite;
}
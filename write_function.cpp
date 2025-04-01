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
	for (UnsignedType i = 0; i < rows; ++i)
	{
		if (displacements[i].size() >= 7)
		{
			fout << "1: " << displacements[i][0] << "  " << "2: " << "  " <<
				displacements[i][1] << "  " << "5: " << displacements[i][4] <<
				"  " << "6: " << displacements[i][6] << "\n";
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
	for (UnsignedType i = 0; i < rows; ++i)
	{
		if (displacements[i].size() >= 1)
			fout << displacements[i][0] << "\n";
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
	for (UnsignedType i = 0; i < rows; ++i)
	{
		if (displacements[i].size() >= 2)
			fout << displacements[i][1] << "\n";
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
	for (UnsignedType i = 0; i < rows; ++i)
	{
		if (displacements[i].size() >= 5)
			fout << displacements[i][4] << "\n";
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
	for (UnsignedType i = 0; i < rows; ++i)
	{
		if (displacements[i].size() >= 7)
			fout << displacements[i][6] << "\n";
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

	for (UnsignedType i = 0; i < steps; ++i)
	{
		fout << deltaT * i << "\n";
	}
	fout.close();

	return isWrite;
}
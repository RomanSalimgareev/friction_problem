#include "write_function.h"

void writeDispAllNodes(const UnsignedType& rows,
	const Matrix<Real>& displacements)
{
	std::ofstream fout("displacements.txt");
	for (UnsignedType i = 0; i < rows; ++i)
	{
		fout << "1: " << displacements[i][0] << "  " << "2: " << "  " <<
		displacements[i][1] << "  " << "5: " <<  displacements[i][4] <<
		"  " << "6: " << displacements[i][6] << "\n";
	}
	fout.close();
}

void writeDispFirstNode(const UnsignedType& rows,
	const Matrix<Real>& displacements)
{
	std::ofstream fout("displacements.txt");
	for (UnsignedType i = 0; i < rows; ++i)
		fout << displacements[i][0] << "\n";

	fout.close();
}

void writeDispSecondNode(const UnsignedType& rows,
	const Matrix<Real>& displacements)
{
	std::ofstream fout("displacements.txt");
	for (UnsignedType i = 0; i < rows; ++i)
		fout << displacements[i][1] << "\n";

	fout.close();
}
void writeDispFifthNode(const UnsignedType& rows,
	const Matrix<Real>& displacements)
{
	std::ofstream fout("displacements.txt");
	for (UnsignedType i = 0; i < rows; ++i)
		fout << displacements[i][4] << "\n";

	fout.close();
}
void writeDispSixthNode(const UnsignedType& rows,
	const Matrix<Real>& displacements)
{
	std::ofstream fout("displacements.txt");
	for (UnsignedType i = 0; i < rows; ++i)
		fout << displacements[i][06] << "\n";

	fout.close();
}

void writeStepsTime(const UnsignedType& steps, const Real& deltaT)
{
	std::ofstream fout("stepsTime.txt");
	for (UnsignedType i = 0; i < steps; ++i)
	{
		fout << deltaT * i << "\n";
	}
	fout.close();
}
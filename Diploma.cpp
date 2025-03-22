//	 The main goal of this program is to solve the dynamic problem of oscillation
// of a parallelepiped-shaped body in a rectangular tube under the action of
// a driving force, taking into account the forces of all-round friction.
//	 The body is affected by comprehensive compression from the walls during the
// entire movement process.
//	 In addition, the program provides solvers for the problem of dry friction
// without a driving force and viscous friction with a driving force to compare
// the results. 
//	 Next, the body is divided into 8 finite elements, after which,
// for simplicity of calculations, only one (marked on the attached photo)
// is considered with the imposition of symmetry conditions.
//   The program sets the properties of the final element, the time,
// the time step for integration, as well as the initial conditions.
// 
// All variables are used in a single calculation system: stresses - pascals,
// density - kg/ m^3, time - seconds, dimensions - meters,
// modulus of elasticity - pascals
// 
// All formulas are given from the book:
// "Голованов А.И., Бережной Д.В. Метод конечных элементов в механике
// деформируемых твердых тел.– Казань : Издательство “ДАС”, 2001. – 301 с."

#include <iostream>
#include <fstream>

#include "dinamic_solver_MFE.h"
#include "matrix_MFE.h"
#include "write_function.h"

int main()
{
	// Material properties: modulus Elastic, coefficient Puasson, dencity
	Real modulusElastic = 7e10;
	Real coefficientPuasson = 0.33;
	Real dencity = 2700.0;

	// As well as the oscillation time and time step (for numerical integration)
	Real time = 2.0;
	Real deltaT = 0.000001;

	// Dimensions of the final element
	Real length = 0.5, width = 0.06, heigth = 0.05;

	char choiceParameters = 'n';
	std::cout << "Do you want to enter the parameters manually? (y/n) \n";
	std::cin >> choiceParameters;
	if (choiceParameters == 'y' or choiceParameters == 'Y')
	{
		std::cout << "Input modulus elastic: " << "\n";
		std::cin >> modulusElastic;

		std::cout << "Input coefficient Puasson: " << "\n";
		std::cin >> coefficientPuasson;

		std::cout << "Input dencity: " << "\n";  
		std::cin >> dencity;

		std::cout << "Input time oscilation: " << "\n";
		std::cin >> time;

		std::cout << "Input time step: " << "\n";
		std::cin >> deltaT;

		std::cout << "Input length final element: " << "\n";
		std::cin >> length;

		std::cout << "Input width final element: " << "\n";
		std::cin >> width;

		std::cout << "Input heigth final element: " << "\n";
		std::cin >> heigth;
	}

	// Matrix stiffness
	RealMatrix matrixStiffness = makeMatrixStiffness(length, width,
		heigth, modulusElastic, coefficientPuasson);

	UnsignedType rows = matrixStiffness.sizeRows();
	// Diagonal matrix mass final element
	RealMatrix matrixMass = 
		makeMatrixMassDiag(rows, dencity, length, width, heigth);

	RealMatrix displacements =
		calculateDisplacementsDinamic (time, deltaT,
			matrixStiffness, matrixMass);

	const UnsignedType rowsD = displacements.sizeRows();
	writeDispAllNodes(rowsD, displacements);

	UnsignedType steps = static_cast<UnsignedType> (time / deltaT);
	writeStepsTime(steps, deltaT);
	
}

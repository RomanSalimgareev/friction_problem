//	 The main goal of this program is to solve the dynamic problem of oscillation
// of a parallelepiped-shaped body in a rectangular tube under the action of
// a driving force, taking into account the forces of all-round friction.
//	 The body is affected by comprehensive compression from the walls during the
// entire movement process.
//	 In addition, the program provides solvers for the problem of dry friction
// without a driving force and viscous friction with a driving force to compare
// the results. The program uses the reflection method to minimize the error.
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
// Material - aluminum
// 
// All formulas are given from the book:
// "Голованов А.И., Бережной Д.В. Метод конечных элементов в механике
// деформируемых твердых тел.– Казань : Издательство “ДАС”, 2001. – 301 с."

#include <iostream>
#include <fstream>

#include <MFE_lib/functions_for_MFE/dinamic_solver_MFE.h>
#include <MFE_lib/functions_for_MFE/matrix_MFE.h>
#include <MFE_lib/write_functions.h>
#include <MFE_lib/get_disps_func.h>
#include "../libs/matplotlib/matplotlibcpp.h"

using namespace MFE;
namespace plt = matplotlibcpp;

int main()
{
	FiniteElement finiteElement;
	chooseMaterialProperties(finiteElement);

	// As well as the oscillation time and time step (for numerical integration)
	Real time = inputTime();
	// Up to 1e-14
	Real deltaT = inputDeltaT();

	try
	{
		// Matrix stiffness
		RealMatrix matrixStiffness = makeMatrixStiffness(finiteElement);

		UnsignedType rows = matrixStiffness.sizeRows();
		// Diagonal matrix mass final element
		RealMatrix matrixMassDiag =
			makeMatrixMassDiag(rows, finiteElement);

		RealMatrix displacements =
			calculateDisplacementsDinamic(time, deltaT,
				matrixStiffness, matrixMassDiag);

		// Writing to "displacements.txt"
		writeDispSecondNode(displacements);

		UnsignedType steps = static_cast<UnsignedType> (time / deltaT);

		RealVector dispsSecondNode = getDispSecondNode(displacements);
		RealVector stepsTime = getStepsTime(steps, deltaT);

		plt::plot(stepsTime, dispsSecondNode);
		plt::title("Displacements node");
		plt::xlabel("X, sec");
		plt::ylabel("Y, m");
		plt::grid(true);
		plt::save("graph.png");
	}
	catch (const std::exception& ex)
	{
		std::cout << "Error : " << ex.what() << "\n";
		std::exit(EXIT_FAILURE);
	}
}

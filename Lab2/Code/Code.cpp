#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

bool isDobuleEqual(double x, double y, int accuracySigns = 5) {
	int intFract = fabs(x - y) * pow(10, accuracySigns);
	return intFract == 0;
}

struct Extrmemum {
	double x;
	double y;
};

int main()
{
	const double a = 0.7;
	const double b = 1.2;
	const double xInitial = 0.5;
	const double xEnd = 1.5;
	const double dx = 0.05;
	
	Extrmemum zMin{
		0, numeric_limits<double>::max()
	};

	Extrmemum zMax{
		0, numeric_limits<double>::min()
	};

	for (double currentX = xInitial; currentX < xEnd or isDobuleEqual(currentX, xEnd); currentX+=dx)
	{
		double currentZ;

		if (currentX < a or isDobuleEqual(currentX, a)) {
			cout << "FN1 | ";
			currentZ = exp(currentX) * sin(currentX);
		}
		else if (currentX > a and currentX < b) {
			cout << "FN2 | ";
			currentZ = tan(currentX) + (currentX * currentX);
		}
		else if (currentX > b or isDobuleEqual(currentX, b)) {
			cout << "FN3 | ";
			currentZ = pow(currentX, 7);
		}

		if (currentZ < zMin.y) {
			zMin.y = currentZ;
			zMin.x = currentX;
		}

		if (currentZ > zMax.y) {
			zMax.y = currentZ;
			zMax.x = currentX;
		}


		std::cout << "Z: [" << currentZ << "]\tX: [" << currentX << "]\n" ;
	}

	std::cout << "\nMinimal Z: [" << zMin.x << ";" << zMin.y << "]\tMaximal Z: [" << zMax.x << ";" << zMax.y << "]\n";
}
#include "helpers.hpp"
#include "vector.hpp"
#include <cmath>

Quadradic solve_quadratic(float a, float b, float c) {
	Quadradic result;

	float	  discr = std::pow(b, 2) - 4 * a * c;
	result.solved = false;
	if (discr < 0)
		return result;

	if (discr == 0) {
		result.x0 = -0.5 * b / a;
		result.x1 = -0.5 * b / a;
	}
	else {
		float q;
		if (b > 0)
			q = -0.5 * (b + sqrt(discr));
		else
			q = -0.5 * (b - sqrt(discr));
		result.x0 = q / a;
		result.x1 = c / q;
	}
	if (result.x0 > result.x1)
		std::swap(result.x0, result.x1);
	result.solved = true;
	return result;
}

ID generateID() {
	static ID id = 0;
	return id++;
}

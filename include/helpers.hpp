#pragma once
#include "types.hpp"

typedef struct {
	bool  solved;
	float x0;
	float x1;
} Quadradic;
Quadradic solve_quadratic(float a, float b, float c);
ID		  generateID();

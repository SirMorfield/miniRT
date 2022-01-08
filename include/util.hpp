#pragma once
#include "types.hpp"
#include <functional>
typedef struct {
	bool  solved;
	float x0;
	float x1;
} Quadradic;
Quadradic solve_quadratic(float a, float b, float c);

template <typename T>
T radians(T degrees);
template <typename T>
T  degrees(T radians);

ID generateID();

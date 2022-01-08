#pragma once
#include "types.hpp"
#include <functional>
#include <sstream>

typedef struct {
	bool  solved;
	float x0;
	float x1;
} Quadradic;
Quadradic solve_quadratic(float a, float b, float c);

template <class T>
T radians(T degrees) {
	return degrees * 3.141592653589793 / 180;
}

template <typename T>
T degrees(T radians) {
	return radians * 180 / 3.141592653589793;
}

ID generateID();

//
std::vector<std::string> split(const std::string& s, char delim);

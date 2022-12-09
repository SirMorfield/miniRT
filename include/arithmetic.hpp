#pragma once

#include "types.hpp"
#include <cmath>
#include <limits>
#include <type_traits>

template <class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y) {
	return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y)
		   // the result is subnormal
		   || std::fabs(x - y) < std::numeric_limits<T>::min();
}

namespace _rand {

float _float(float a, float b);
Vec3  vec3(float a, float b);

} // namespace _rand

template <class T>
T radians(T degrees) {
	return degrees * 3.141592653589793 / 180;
}

template <typename T>
T degrees(T radians) {
	return radians * 180 / 3.141592653589793;
}

// Returns the closest prime number where: result <= n
template <typename T>
T closest_prime(T n, std::enable_if<std::is_integral<T>::value>* = nullptr) {
	// All prime numbers are odd except two
	if (n & 1)
		n -= 2;
	else
		n--;

	T i, j;
	for (i = n; i >= 2; i -= 2) {
		if (i % 2 == 0)
			continue;
		for (j = 3; j <= std::sqrt(i); j += 2) {
			if (i % j == 0)
				break;
		}
		if (j > std::sqrt(i))
			return i;
	}
	// It will only be executed when n is 3
	return 2;
}

template <typename T>
bool is_power_of_2(T x, std::enable_if<std::is_integral<T>::value>* = nullptr) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

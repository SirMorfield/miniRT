#include "types.hpp"

namespace _rand {

float _float(float a, float b) {
	return ((b - a) * ((float)rand() / (float)RAND_MAX)) + a;
}

Vec3 vec3(float a, float b) {
	return Vec3(_float(a, b), _float(a, b), _float(a, b));
}

} // namespace _rand

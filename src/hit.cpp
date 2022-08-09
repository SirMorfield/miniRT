#include "types.hpp"

Hit::Hit(bool hit) : hit(hit) {
}

Hit::Hit(float dist, const Vec3& point, const Vec3& normal, const Rgb& color)
	: hit(true),
	  dist(dist),
	  point(point),
	  normal(normal),
	  color(color) {}

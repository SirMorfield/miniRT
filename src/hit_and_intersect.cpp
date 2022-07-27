#include "types.hpp"

Intersect::Intersect(bool hit) : hit(hit) {
}

Intersect::Intersect(float dist, Vec3 point, Vec3 normal) : hit(true), dist(dist), point(point), normal(normal) {
}

Hit::Hit(bool hit) : Intersect(hit) {
}

Hit::Hit(float dist, Vec3 point, Vec3 normal, Rgb color, ID id) : Intersect(dist, point, normal) {
	this->color = color;
	this->id = id;
}

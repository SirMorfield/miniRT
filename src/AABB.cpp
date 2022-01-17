#include "shapes.hpp"

AABB::AABB(const Vec3& min, const Vec3& max) : _min(min), _max(max) {
}

bool AABB::intersect(const Ray& ray) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (ray.dir.x >= 0) {
		tmin = (_min.x - ray.origin.x) / ray.dir.x;
		tmax = (_max.x - ray.origin.x) / ray.dir.x;
	} else {
		tmin = (_max.x - ray.origin.x) / ray.dir.x;
		tmax = (_min.x - ray.origin.x) / ray.dir.x;
	}
	if (ray.dir.y >= 0) {
		tymin = (_min.y - ray.origin.y) / ray.dir.y;
		tymax = (_max.y - ray.origin.y) / ray.dir.y;
	} else {
		tymin = (_max.y - ray.origin.y) / ray.dir.y;
		tymax = (_min.y - ray.origin.y) / ray.dir.y;
	}
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	if (ray.dir.z >= 0) {
		tzmin = (_min.z - ray.origin.z) / ray.dir.z;
		tzmax = (_max.z - ray.origin.z) / ray.dir.z;
	} else {
		tzmin = (_max.z - ray.origin.z) / ray.dir.z;
		tzmax = (_min.z - ray.origin.z) / ray.dir.z;
	}
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return tmax > 0;
}

bool AABB::isInside(const Vec3& p) const {
	return (p.x >= _min.x && p.x <= _max.x) &&
		   (p.y >= _min.y && p.y <= _max.y) &&
		   (p.z >= _min.z && p.z <= _max.z);
}

AABB::~AABB() {}

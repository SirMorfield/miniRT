#include "shapes.hpp"
#include "types.hpp"
USE_EIGEN
Triangle::Triangle(Rgb color, Vec3 p0, Vec3 p1, Vec3 p2) {
	this->id = generateID();
	this->color = color;
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
#ifdef USE_EIGEN
	this->edge1 = p1 - p0;
	this->edge2 = p2 - p0;
	this->normal = edge1.cross(edge2);
	this->normal.normalize();
#endif
}

Hit Triangle::hit(const Ray& ray) const {
	Vec3   h, s, q;
	double a, f, u, v;

#ifndef USE_EIGEN
	Vec3 edge1 = p1 - p0;
	Vec3 edge2 = p2 - p0;
	Vec3 normal = edge1.cross(edge2);
	normal.normalize();
#endif

	h = ray.dir.cross(edge2);
	a = edge1.dot(h);
	if (a > -EPSILON && a < EPSILON)
		return Hit(false); // Ray is parallel to this triangle.
	f = 1.0 / a;
	s = ray.origin - p0;
	u = f * s.dot(h);
	if (u < 0.0 || u > 1.0)
		return Hit(false);
	q = s.cross(edge1);
	v = f * ray.dir.dot(q);
	if (v < 0.0 || u + v > 1.0)
		return Hit(false);
	double t = f * edge2.dot(q);
	if (t < EPSILON) // There is a line intersection but not a ray intersection
		return Hit(false);

	Hit hit(true);
	hit.dist = (ray.dir * t).length();
	hit.point = ray.origin * hit.dist;
	hit.normal = correct_normal(normal, ray);
	hit.color = color;
	return (hit);
}

bool Triangle::is_inside_AABB(const AABB& aabb) const {
	return aabb.is_inside(p0) &&
		   aabb.is_inside(p1) &&
		   aabb.is_inside(p2);
}

AABB Triangle::get_AABB() const {
	Vec3 min(
		std::min({p0.x(), p1.x(), p2.x()}),
		std::min({p0.y(), p1.y(), p2.y()}),
		std::min({p0.z(), p1.z(), p2.z()}));
	Vec3 max(
		std::max({p0.x(), p1.x(), p2.x()}),
		std::max({p0.y(), p1.y(), p2.y()}),
		std::max({p0.z(), p1.z(), p2.z()}));

	return AABB(min, max);
}

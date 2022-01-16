#include "shapes.hpp"
#include "types.hpp"

Triangle::Triangle(Rgb color, Vec3 p0, Vec3 p1, Vec3 p2) {
	this->id = generateID();
	this->color = color;
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->edge1 = p1 - p0;
	this->edge2 = p2 - p0;
	this->normal = (p1 - p0).cross(p2 - p0);
	this->normal.normalize();
}

Intersect Triangle::intersect(const Ray& ray) const {
	Vec3   h, s, q;
	double a, f, u, v;

	h = ray.dir.cross(edge2);
	a = edge1.dot(h);
	if (a > -EPSILON && a < EPSILON)
		return Intersect(false); // Ray is parallel to this triangle.
	f = 1.0 / a;
	s = ray.origin - p0;
	u = f * s.dot(h);
	if (u < 0.0 || u > 1.0)
		return Intersect(false);
	q = s.cross(edge1);
	v = f * ray.dir.dot(q);
	if (v < 0.0 || u + v > 1.0)
		return Intersect(false);
	double t = f * edge2.dot(q);
	if (t < EPSILON) // There is a line intersection but not a ray intersection
		return Intersect(false);

	Intersect intersect(true);
	intersect.dist = (ray.dir * t).length();
	intersect.point = ray.origin * intersect.dist;
	intersect.normal = correct_normal(normal, ray);
	return (intersect);
}

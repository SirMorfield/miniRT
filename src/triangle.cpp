#include "shapes.hpp"
#include "types.hpp"

Triangle::Triangle(Rgb color, Vec3 p0, Vec3 p1, Vec3 p2) {
	this->id = generateID();
	this->color = color;
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->edge1 = subtract(p1, p0);
	this->edge2 = subtract(p2, p0);
	this->normal = unit(cross(subtract(p1, p0), subtract(p2, p0)));
}

Intersect Triangle::intersect(const Ray& ray) const {
	Vec3   h, s, q;
	double a, f, u, v;

	h = cross(ray.dir, edge2);
	a = dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return Intersect(false); // Ray is parallel to this triangle.
	f = 1.0 / a;
	s = subtract(ray.origin, p0);
	u = f * dot(s, h);
	if (u < 0.0 || u > 1.0)
		return Intersect(false);
	q = cross(s, edge1);
	v = f * dot(ray.dir, q);
	if (v < 0.0 || u + v > 1.0)
		return Intersect(false);
	double t = f * dot(edge2, q);
	if (t < EPSILON) // There is a line intersection but not a ray intersection
		return Intersect(false);

	Intersect intersect(true);
	intersect.dist = length(scale(ray.dir, t));
	intersect.point = scale(ray.origin, intersect.dist);
	intersect.normal = normal;
	return (intersect);
}

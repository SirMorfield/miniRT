#include "shapes.hpp"
#include "types.hpp"
#include "util.hpp"

Sphere::Sphere(const Vec3& origin, const Rgb& color, float radius) {
	this->color = color;
	this->id = generateID();
	this->_origin = origin;
	this->_radius = radius;
}

Quadradic Sphere::get_hitions(const Ray& ray) const {
	Vec3  l = ray.origin - _origin;
	float a = ray.dir.dot(ray.dir);
	float b = 2 * ray.dir.dot(l);
	float c = l.dot(l) - _radius * _radius;
	return solve_quadratic(a, b, c);
}

// see doc/rayspherecases.png
static float actual_t(float t0, float t1) {
	if (t0 > 0 && t1 > 0)
		return (t0);
	if (t0 <= 0 && t1 > 0)
		return (t1);
	return (-1);
}

Hit Sphere::hit(const Ray& ray) const {
	Quadradic q = get_hitions(ray);
	if (!q.solved)
		return Hit(false);
	Hit hit(true);
	hit.dist = actual_t(q.x0, q.x1);
	if (hit.dist < 0)
		return Hit(false);
	hit.point = ray.origin;
	hit.point.translate(ray.dir, hit.dist);
	if (ray.origin.distance2(_origin) > _radius * _radius)
		hit.normal = hit.point - _origin;
	else
		hit.normal = _origin - hit.point;
	hit.normal.normalize();
	hit.color = color;
	return hit;
}

bool Sphere::is_inside_AABB(const AABB& aabb) const {
	if (!aabb.is_inside(_origin))
		return false;

	return _origin.distance2(aabb._min) > _radius * _radius &&
		   _origin.distance2(aabb._max) > _radius * _radius;
}

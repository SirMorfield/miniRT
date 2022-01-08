#include "main.hpp"
#include "util.hpp"
#include "vector.hpp"

Sphere::Sphere(const Vec3& origin, const Rgb& color, float radius) {
	this->color = color;
	this->_origin = origin;
	this->_radius = radius;
}

Quadradic Sphere::_get_intersections(const Ray& ray) const {
	Vec3  l = subtract(ray.origin, _origin);
	float a = dot(ray.dir, ray.dir);
	float b = 2 * dot(ray.dir, l);
	float c = dot(l, l) - _radius * _radius;
	return (solve_quadratic(a, b, c));
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
	Quadradic q = _get_intersections(ray);
	if (!q.solved)
		return Hit(false);
	Hit hit(true);
	hit.dist = actual_t(q.x0, q.x1);
	if (hit.dist < 0)
		return Hit(false);
	hit.point = translate(ray.origin, ray.dir, hit.dist);
	if (distance2(ray.origin, _origin) > _radius * _radius)
		hit.normal = unit(subtract(hit.point, _origin));
	else
		hit.normal = unit(subtract(_origin, hit.point));
	hit.color = color;
	return hit;
}

#pragma once
#include "lighting.hpp"
#include "types.hpp"
#include "util.hpp"

class Shape {
  public:
	virtual Intersect intersect(const Ray& ray) const = 0;
	Rgb				  color;
	ID				  id;
};

class Sphere : public Shape {
  public:
	Sphere(const Vec3& origin, const Rgb& color, float radius);
	Intersect intersect(const Ray& ray) const;
	virtual ~Sphere() {}

  private:
	Vec3	  _origin;
	float	  _radius;
	Quadradic _get_intersections(const Ray& ray) const;
};

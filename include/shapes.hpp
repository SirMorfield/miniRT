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

class Triangle : public Shape {
  public:
	Triangle(Rgb color, Vec3 p0, Vec3 p1, Vec3 p2);
	Intersect intersect(const Ray& ray) const;
	virtual ~Triangle() {}

  private:
	Vec3 p0;
	Vec3 p1;
	Vec3 p2;
	Vec3 edge1;
	Vec3 edge2;
	Vec3 normal;
};

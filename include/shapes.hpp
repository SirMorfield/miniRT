#pragma once
#include "helpers.hpp"
#include "lighting.hpp"
#include "types.hpp"
#include "util.hpp"

class AABB;

class Shape {
  public:
	virtual Intersect intersect(const Ray& ray) const = 0;
	virtual bool	  is_inside_AABB(const AABB& aabb) const = 0;
	Rgb				  color;
	ID				  id;
};

class Sphere : public Shape {
  public:
	Sphere(const Vec3& origin, const Rgb& color, float radius);
	Intersect intersect(const Ray& ray) const;
	bool	  is_inside_AABB(const AABB& aabb) const;

	virtual ~Sphere() {}

  private:
	Vec3	  _origin;
	float	  _radius;
	Quadradic get_intersections(const Ray& ray) const;
};

class Triangle : public Shape {
  public:
	Triangle(Rgb color, Vec3 p0, Vec3 p1, Vec3 p2);
	Intersect intersect(const Ray& ray) const;
	bool	  is_inside_AABB(const AABB& aabb) const;

	virtual ~Triangle() {}

  private:
	Vec3 p0;
	Vec3 p1;
	Vec3 p2;
	Vec3 edge1;
	Vec3 edge2;
	Vec3 normal;
};

class AABB {
  public:
	AABB(const Vec3& min, const Vec3& max);
	bool intersect(const Ray& ray) const;
	bool is_inside(const Vec3& p) const;

	// subdivide space into  8 children
	std::vector<AABB> subdivide() const;

	// bool			  is_inside_AABB(const AABB& aabb) const; // TODO: instead of shape.is_inside_AABB(aabb)

	//   private:
	Vec3 _min;
	Vec3 _max;
};

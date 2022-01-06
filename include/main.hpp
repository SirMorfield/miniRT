#pragma once
#include <functional>
#include <iostream>
#include <vector>

class Vec3 {
  public:
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
	Vec3() {}
};

// typedef struct {
// 	Vec3 origin;
// 	Vec3 dir;
// } Ray;

class Rgb {
  public:
	Rgb() {}
	Rgb(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class Hit {
  public:
	Hit(bool hit, float dist, Vec3 point, Vec3 normal) : hit(hit), dist(dist), point(point), normal(normal) {}
	bool  hit;
	float dist;
	Vec3  point;
	Vec3  normal;
};

class Shape {
  public:
	virtual Hit hit(const Vec3& origin, const Vec3& dir) = 0;
	Vec3		position;
	Rgb			color;
};

class Sphere : public Shape {
  public:
	Sphere(const Vec3& position, const Rgb& color, float radius) {
		this->position = position;
		this->color = color;
		this->radius = radius;
	}
	Hit hit(const Vec3& origin, const Vec3& dir);
	virtual ~Sphere() {}
	float radius;
};

class Scene {
  public:
	Scene();

  private:
	std::vector<Sphere> _spheres;
	// forbidden
	// Scene& operator=(const Scene& cp);
	// Scene(const Scene& cp);
};

#pragma once
#include "util.hpp"
#include <algorithm>
#include <cmath>
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

typedef struct {
	Vec3 origin;
	Vec3 dir;
} Ray;

typedef struct { // TODO better name
	float t0;
	float t1;
} Tintesects;

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
	Hit(bool hit) : hit(hit) {}
	Hit(bool hit, float dist, Vec3 point, Vec3 normal) : hit(hit), dist(dist), point(point), normal(normal) {}
	bool  hit;
	float dist;
	Vec3  point;
	Vec3  normal;
};

class Shape {
  public:
	virtual Hit hit(const Ray& ray) const = 0;
	Rgb			color;
};

class Camera {
  public:
	Camera() {}
	Camera(Vec3 pos, Vec3 dir, float fov);

	//   private:
	Vec3  _pos;
	Vec3  _dir;
	float _fov;
};

class Sphere : public Shape {
  public:
	Sphere(const Vec3& origin, const Rgb& color, float radius) {
		this->color = color;
		this->_origin = origin;
		this->_radius = radius;
	}
	Hit hit(const Ray& ray) const;
	virtual ~Sphere() {}

  private:
	Vec3	  _origin;
	float	  _radius;
	Quadradic _get_intersections(const Ray& ray) const;
};

class Scene {
  public:
	Scene();
	Hit hit(const Ray& ray) const;

  private:
	std::vector<Sphere> _spheres;
};

class Renderer {
  public:
	Renderer(size_t xSize, size_t ySize);
	void render(const Scene& scene);
	void saveToBMP();

  private:
	Ray				 _rayFromPixel(float x, float y);
	size_t			 _xSize;
	size_t			 _ySize;
	Camera			 _camera;
	float			 _aspectRatio;
	float			 _fov;
	std::vector<Rgb> _frame;
};

bool save_bmp(size_t xSize, size_t ySize, const std::vector<Rgb>& frame, const std::string path);

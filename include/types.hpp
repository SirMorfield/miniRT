#pragma once
#include "vector.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#define EPSILON 1e-8

class Rgb {
  public:
	Rgb() {}
	Rgb(uint8_t r, uint8_t g, uint8_t b);
	void	add(const Rgb& color, float intensity);

	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class Ray {
  public:
	Ray() {}
	Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir) {}
	Vec3 origin;
	Vec3 dir;
};

typedef struct { // TODO better name
	float t0;
	float t1;
} Tintesects;

typedef uint64_t ID;

class Intersect {
  public:
	Intersect(bool hit);
	Intersect(float dist, Vec3 point, Vec3 normal);
	bool  hit;
	float dist;
	Vec3  point;
	Vec3  normal;
};

class Hit : public Intersect {
  public:
	Hit(bool hit);
	Hit(float dist, Vec3 point, Vec3 normal, Rgb color, ID id);
	Rgb color;
	ID	id;
};

class Light {
  public:
	Light(const Rgb& color, const Vec3& origin, float brightness);
	float relative_intensity(const Vec3& point, const Vec3& normal) const;

	//   private:
	Rgb	  _color;
	Vec3  _origin;
	float _brightness;
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

class Resolution {
  public:
	Resolution() = delete;
	Resolution(size_t width, size_t height) : _width(width), _height(height) {}

	size_t width() const { return _width; }
	size_t height() const { return _height; }

  private:
	size_t _width;
	size_t _height;
};

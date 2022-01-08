#pragma once
#include "vector.hpp"
#include <functional>
#include <string>
#include <vector>

class Vec3 {
  public:
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
	Vec3() {}
};

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
	float relativeIntensity(const Vec3& point, const Vec3& normal) const;

	//   private:
	Rgb	  _color;
	Vec3  _origin;
	float _brightness;
};

#pragma once
#include <functional>

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

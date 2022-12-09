#pragma once
#include "vector.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#define EPSILON 1e-8

typedef struct { // TODO better name
	float t0;
	float t1;
} Tintesects;

typedef uint64_t ID;

// TODO: maybe as optional?
class Hit {
  public:
	Hit(bool hit);
	Hit(float dist, const Vec3& point, const Vec3& normal, const Rgb& color);

	// bool		hit() const { return _hit; }
	// float		dist() const { return _dist; }
	// const Vec3& point() const { return _point; }
	// const Vec3& normal() const { return _normal; }
	// const Rgb&	color() const { return _color; }

	//   private:
	bool  hit;
	float dist;
	Vec3  point;
	Vec3  normal;
	Rgb	  color;
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
	float _fov_tan;
};

class Resolution {
  public:
	enum class Standard_size {
		R_8K,
		R_5K,
		R_4K,
		R_1080p,
		R_720p,
		R_480p,
		R_360p,
		R_240p,
		R_144p,
	};

	Resolution() = delete;
	Resolution(size_t width, size_t height, size_t AA_level);
	Resolution(Resolution::Standard_size standard_size, size_t AA_level);

	size_t width() const { return _width; }
	size_t height() const { return _height; }
	size_t AA_level() const { return _AA_level; }

  private:
	size_t _width;
	size_t _height;
	size_t _AA_level;
};
std::ostream& operator<<(std::ostream& os, const Resolution& res);
;

#pragma once
#include "types.hpp"

class Rgb {
  public:
	Rgb() {}
	Rgb(uint8_t r, uint8_t g, uint8_t b);
	void	add(const Rgb& color, float intensity);

	uint8_t r;
	uint8_t g;
	uint8_t b;
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

Rgb mixColor(const Rgb& light, const Rgb& surface);

#include "types.hpp"
#include <algorithm>
#include <cmath>

//Rgb::
Rgb::Rgb(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

void Rgb::add(const Rgb& color, float intensity) {
	r = std::round(std::min(r + color.r * intensity, 255.0f));
	g = std::round(std::min(g + color.g * intensity, 255.0f));
	b = std::round(std::min(b + color.b * intensity, 255.0f));
}

// Light::

Light::Light(const Rgb& color, const Vec3& origin, float brightness) : _color(color), _origin(origin), _brightness(brightness) {}

float Light::relativeIntensity(const Vec3& point, const Vec3& normal) const {
	Vec3  to_light = unit(subtract(_origin, point));
	float intensity = 1 * _brightness * std::max(dot(normal, to_light), 0.0f);
	return std::min(intensity, 1.0f);
}

// other
Rgb mixColor(const Rgb& light, const Rgb& surface) {
	return Rgb(std::round(light.r * (surface.r / 255.0)),
			   std::round(light.g * (surface.g / 255.0)),
			   std::round(light.b * (surface.b / 255.0)));
}

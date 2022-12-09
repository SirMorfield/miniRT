#include "types.hpp"
#include <algorithm>
#include <cmath>

// Light::

Light::Light(const Rgb& color, const Vec3& origin, float brightness) : _color(color), _origin(origin), _brightness(brightness) {}

float Light::relative_intensity(const Vec3& point, const Vec3& normal) const {
	Vec3 to_light = _origin - point;
	to_light.normalize();
	float intensity = 1 * _brightness * std::max(normal.dot(to_light), 0.0f);
	return std::min(intensity, 1.0f);
}

// other
Rgb mix_color(const Rgb& light, const Rgb& surface) {
	return Rgb(std::round(light.x() * (surface.x() / 255.0)),
			   std::round(light.y() * (surface.y() / 255.0)),
			   std::round(light.z() * (surface.z() / 255.0)));
}

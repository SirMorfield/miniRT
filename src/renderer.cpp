#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cmath>

Renderer::Renderer(const Resolution& resolution)
	: _resolution(resolution) {
	_aspect_ratio = (float)_resolution.width() / _resolution.height();
}

Ray Renderer::ray_from_pixel(const Camera& camera, float x, float y) const {
	float px = (2 * x / _resolution.width() - 1) * _aspect_ratio * camera._fov_tan;
	float py = (2 * y / (float)_resolution.height() - 1) * camera._fov_tan;
	Ray	  ray;
	ray.origin = camera._pos;
	Vec3 positiveX;
	if (camera._dir.x() == 0.0 && camera._dir.z() == 0.0)
		positiveX = Vec3(1.0, 0.0, 0.0);
	else
		positiveX = camera._dir.cross(Vec3(0.0, 1.0, 0.0));
	Vec3 negativeY = camera._dir.cross(positiveX);
	ray.dir = ((positiveX * px) + (negativeY * py)) + camera._dir;
	ray.dir.normalize();
	return ray;
}

Rgb average_color(const std::vector<Rgb>& colors) {
	Vec3 color(0);

	for (const Rgb& c : colors)
		color += Vec3(c.x(), c.y(), c.z());

	color /= colors.size();
	return Rgb(std::round(color.x()),
			   std::round(color.y()),
			   std::round(color.z()));
}

void Renderer::thread(const Scene& scene, Frame_buffer* fb) {
	std::vector<Rgb> colors(_resolution.AA_level() * _resolution.AA_level());
	const float		 aa = 1.0f / std::sqrt(_resolution.AA_level());

	while (true) {
		std::optional<Vec<size_t, 2>> pixel = fb->get_pixel();
		if (!pixel.has_value())
			break;

		for (float y_off = EPSILON; y_off < 1.0f; y_off += aa) {
			for (float x_off = EPSILON; x_off < 1.0f; x_off += aa) {
				Ray ray = ray_from_pixel(scene._camera, pixel->x() + x_off, pixel->y() + y_off);
				colors.push_back(scene.get_color(ray));
			}
		}
		fb->set_pixel(average_color(colors), pixel->x(), pixel->y());
		colors.clear();
	}
}

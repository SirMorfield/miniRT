#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cmath>

Renderer::Renderer(size_t xSize, size_t ySize) {
	_x_size = xSize;
	_y_size = ySize;
	_aspect_ratio = (float)_x_size / _y_size;
}

Ray Renderer::ray_from_pixel(const Camera& camera, float x, float y) const {
	float px = (2 * x / _x_size - 1) * _aspect_ratio * camera._fov_tan;
	float py = (2 * y / (float)_y_size - 1) * camera._fov_tan;
	Ray	  ray;
	ray.origin = camera._pos;
	Vec3 positiveX;
	if (camera._dir.x == 0.0 && camera._dir.z == 0.0)
		positiveX = Vec3(1.0, 0.0, 0.0);
	else
		positiveX = camera._dir.cross(Vec3(0.0, 1.0, 0.0));
	Vec3 negativeY = camera._dir.cross(positiveX);
	ray.dir = ((positiveX * px) + (negativeY * py)) + camera._dir;
	ray.dir.normalize();
	return ray;
}

void Renderer::thread(const Scene& scene, Frame_buffer* fb) {
	while (true) {
		std::optional<Point2<size_t>> pixel = fb->get_pixel();
		if (!pixel.has_value())
			break;

		Ray ray = ray_from_pixel(scene._camera, pixel->x, pixel->y);
		Rgb color = scene.get_color(ray);
		fb->set_pixel(color, pixel->x, pixel->y);
	}
}

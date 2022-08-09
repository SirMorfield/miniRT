#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cmath>

Frame_buffer::Frame_buffer(size_t xSize, size_t ySize) {
	_y_size = ySize;
	_x_size = xSize;
	_max_i = xSize * ySize;
	_frame.resize(xSize * ySize);
	_i = 0;
}

std::optional<Point2<size_t>> Frame_buffer::get_pixel() {
	_mutex.lock();
	std::optional<Point2<size_t>> pixel;
	if (_i < _max_i) {
		pixel = Point2<size_t>(_i % _x_size, _i / _x_size);
		_i++;
	}
	_mutex.unlock();
	return pixel;
}

void Frame_buffer::set_pixel(const Rgb& color, size_t x, size_t y) {
	_frame[y * _x_size + x] = color;
}

void Frame_buffer::save_to_BMP() {
	save_bmp(_x_size, _y_size, _frame, "scene.bmp");
}

Renderer::Renderer(size_t xSize, size_t ySize) {
	_x_size = xSize;
	_y_size = ySize;
	_aspect_ratio = (float)_x_size / _y_size;
}

Ray Renderer::ray_from_pixel(const Camera& camera, float x, float y) const {
	float fov = tan(camera._fov * 0.5);
	float px = (2 * x / _x_size - 1) * _aspect_ratio * fov;
	float py = (2 * y / (float)_y_size - 1) * fov;
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

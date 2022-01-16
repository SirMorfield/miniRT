#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cmath>

FrameBuffer::FrameBuffer(size_t xSize, size_t ySize) {
	_ySize = ySize;
	_xSize = xSize;
	_maxI = xSize * ySize;
	_frame.reserve(xSize * ySize);
	_i = 0;
}

bool FrameBuffer::getPixel(size_t& x, size_t& y) {
	bool success;
	_mutex.lock();
	if (_i < _maxI) {
		x = _i % _xSize;
		y = _i / _xSize;
		_i++;
		success = true;
	} else
		success = false;
	_mutex.unlock();
	return success;
}

void FrameBuffer::setPixel(const Rgb& color, size_t x, size_t y) {
	_frame[y * _xSize + x] = color;
}

void FrameBuffer::saveToBMP() {
	save_bmp(_xSize, _ySize, _frame, "scene.bmp");
}

Renderer::Renderer(size_t xSize, size_t ySize) {
	_xSize = xSize;
	_ySize = ySize;
	_aspectRatio = (float)_xSize / _ySize;
}

Ray Renderer::_rayFromPixel(const Camera& camera, float x, float y) const {
	float fov = tan(camera._fov * 0.5);
	float px = (2 * x / _xSize - 1) * _aspectRatio * fov;
	float py = (2 * y / (float)_ySize - 1) * fov;
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

void Renderer::thread(const Scene& scene, FrameBuffer* fb) {
	size_t x;
	size_t y;
	while (fb->getPixel(x, y)) {
		Ray ray = _rayFromPixel(scene._camera, x, y);
		Rgb color = scene.getColor(ray);
		fb->setPixel(color, x, y);
	}
}

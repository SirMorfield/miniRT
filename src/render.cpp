#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cmath>

Renderer::Renderer(size_t xSize, size_t ySize) : _xSize(xSize), _ySize(ySize) {
	_camera = Camera(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, -1.0f), 10.0f);
	_aspectRatio = (float)_xSize / _ySize;
	_fov = tan(_camera._fov * 0.5);
	_frame.reserve(xSize * ySize);
}

Ray Renderer::_rayFromPixel(float x, float y) {
	float px = (2 * x / _xSize - 1) * _aspectRatio * _fov;
	float py = (2 * y / (float)_ySize - 1) * _fov;
	Ray	  ray;
	ray.origin = _camera._pos;
	Vec3 positiveX;
	if (_camera._dir.x == 0.0 && _camera._dir.z == 0.0)
		positiveX = Vec3(1.0, 0.0, 0.0);
	else
		positiveX = cross(_camera._dir, Vec3(0.0, 1.0, 0.0));
	Vec3 negativeY = cross(_camera._dir, positiveX);
	ray.dir = add(add(scale(positiveX, px), scale(negativeY, py)), _camera._dir);
	normalize(&ray.dir);
	return ray;
}

void Renderer::render(const Scene& scene) {
	for (size_t y = 0; y < _ySize; y++) {
		for (size_t x = 0; x < _xSize; x++) {
			Ray ray = _rayFromPixel(x, y);
			_frame[y * _xSize + x] = scene.getColor(ray);
		}
	}
}

void Renderer::saveToBMP() {
	save_bmp(_xSize, _ySize, _frame, "scene.bmp");
}

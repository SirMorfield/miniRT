#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cmath>

Renderer::Renderer(size_t xSize, size_t ySize) : _xSize(xSize), _ySize(ySize) {
	_aspectRatio = (float)_xSize / _ySize;
	_frame.reserve(xSize * ySize);
}

Ray Renderer::_rayFromPixel(const Camera& camera, float x, float y) {
	float fov = tan(camera._fov * 0.5);
	float px = (2 * x / _xSize - 1) * _aspectRatio * fov;
	float py = (2 * y / (float)_ySize - 1) * fov;
	Ray	  ray;
	ray.origin = camera._pos;
	Vec3 positiveX;
	if (camera._dir.x == 0.0 && camera._dir.z == 0.0)
		positiveX = Vec3(1.0, 0.0, 0.0);
	else
		positiveX = cross(camera._dir, Vec3(0.0, 1.0, 0.0));
	Vec3 negativeY = cross(camera._dir, positiveX);
	ray.dir = add(add(scale(positiveX, px), scale(negativeY, py)), camera._dir);
	normalize(&ray.dir);
	return ray;
}

void Renderer::render(const Scene& scene) {
	for (size_t y = 0; y < _ySize; y++) {
		for (size_t x = 0; x < _xSize; x++) {
			Ray ray = _rayFromPixel(scene._camera, x, y);
			_frame[y * _xSize + x] = scene.getColor(ray);
		}
	}
}

void Renderer::saveToBMP() {
	save_bmp(_xSize, _ySize, _frame, "scene.bmp");
}

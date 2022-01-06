#include "main.hpp"
#include <functional>

Scene::Scene() {
	Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), Rgb(100, 100, 100), 10.0f);
	_spheres.push_back(sphere);
}

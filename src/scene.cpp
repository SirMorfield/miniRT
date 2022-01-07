#include "main.hpp"
#include <functional>

Scene::Scene() {
	Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), Rgb(100, 100, 100), 1.0f);
	_spheres.push_back(sphere);
}

Hit Scene::hit(const Ray& ray) const {
	for (std::vector<const Sphere>::iterator it = _spheres.begin(); it != _spheres.end(); ++it) {
		Hit hit = it->hit(ray);
		if (hit.hit)
			return hit;
	}
	return Hit(false);
}

#include "computer.hpp"
#include "types.hpp"
#include <functional>

Scene::Scene() {
	_backgroundColor = Rgb(0, 0, 0);
	Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), Rgb(200, 200, 100), 1.0f);
	_spheres.push_back(sphere);
	Light light(Rgb(255, 255, 0), Vec3(2.0f, 0.0f, 5.0f), 0.8f);
	_lights.push_back(light);
}

Hit Scene::hit(const Ray& ray, const std::set<ID>& ignore) const {
	for (std::vector<const Sphere>::iterator it = _spheres.begin(); it != _spheres.end(); ++it) {
		if (ignore.find(it->id) != ignore.end())
			continue;
		Intersect intersect = it->intersect(ray);
		if (intersect.hit)
			return Hit(intersect.dist, intersect.point, intersect.normal, it->color, it->id);
	}
	return Hit(false);
}

// ray.origin = l->origin;
// ray.dir = unit(subtract(hitpoint, l->origin));
// from_light = get_bounce(shapes, ray);

Rgb Scene::getColor(const Ray& ray) const {
	std::set<ID> ignore;
	Hit			 hit = this->hit(ray, ignore);
	if (!hit.hit)
		return _backgroundColor;
	ignore.insert(hit.id);
	Rgb lightAccumulator = Rgb(0, 0, 0);
	for (std::vector<const Light>::iterator light = _lights.begin(); light != _lights.end(); ++light) {
		float relativeIntensity = light->relativeIntensity(hit.point, hit.normal);
		lightAccumulator.add(light->_color, relativeIntensity);
	}
	return mixColor(lightAccumulator, hit.color);
}

#include "computer.hpp"
#include "io.hpp"
#include "types.hpp"
#include <fstream>
#include <functional>

Scene::Scene() {
	_backgroundColor = Rgb(0, 0, 0);
	Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), Rgb(200, 200, 100), 1.0f);
	_spheres.push_back(sphere);
	Light light(Rgb(255, 255, 0), Vec3(2.0f, 0.0f, 5.0f), 0.8f);
	_lights.push_back(light);
	_camera = Camera(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, -1.0f), 10.0f);
}

Scene::Scene(const std::string& path) {
	_backgroundColor = Rgb(0, 0, 0);

	std::ifstream file(path);
	if (!file.is_open())
		throw "file not found";
	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> blocks = split(line, ' ');
		toLight(blocks, _lights);
		toCamera(blocks, _camera);
		toSphere(blocks, _spheres);
		toTriangle(blocks, _triangles);
	}
	file.close();
}

Hit Scene::hit(const Ray& ray, const std::set<ID>& ignore) const {
	Hit hits[] = {
		hitShape(_spheres, ray, ignore),
		hitShape(_triangles, ray, ignore),
	};
	Hit hit(false);
	for (size_t i = 0; i < sizeof(hits) / sizeof(Hit); i++) {
		if (hits[i].hit && (!hit.hit || hits[i].dist < hit.dist))
			hit = hits[i];
	}
	return hit;
}

bool Scene::isClearPath(const std::set<ID>& ignore, const Vec3& point, const Light& light) const {
	Ray toLight(point, unit(subtract(light._origin, point)));
	Hit hit = Scene::hit(toLight, ignore);
	return !hit.hit || hit.dist * hit.dist > distance2(light._origin, point);
}

Rgb Scene::getColor(const Ray& ray) const {
	std::set<ID> ignore;
	Hit			 hit = this->hit(ray, ignore);
	if (!hit.hit)
		return _backgroundColor;
	ignore.insert(hit.id);
	Rgb lightAccumulator = Rgb(0, 0, 0);
	for (std::vector<const Light>::iterator light = _lights.begin(); light != _lights.end(); ++light) {
		if (!isClearPath(ignore, hit.point, *light))
			continue;
		float relativeIntensity = light->relativeIntensity(hit.point, hit.normal);
		lightAccumulator.add(light->_color, relativeIntensity);
	}
	return mixColor(lightAccumulator, hit.color);
}

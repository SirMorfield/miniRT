#include "computer.hpp"
#include "env.hpp"
#include "io.hpp"
#include "shapes.hpp"
#include "types.hpp"
#include <fstream>
#include <functional>

Scene::Scene() : resolution(env::width, env::height, env::anti_aliasing_level) {
	_backgroundColor = Rgb(0, 0, 0);
	Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), Rgb(200, 200, 100), 1.0f);
	_spheres.push_back(sphere);
	Light light(Rgb(255, 255, 0), Vec3(2.0f, 0.0f, 5.0f), 0.8f);
	_lights.push_back(light);
	_camera = Camera(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, -1.0f), 10.0f);
}

bool is_comment(const std::string& line) {
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == '#')
			return true;

		if (!std::isspace(line[i]))
			break;
	}
	return false;
}

Scene::Scene(const std::string& path) : resolution(env::width, env::height, env::anti_aliasing_level) {
	const AABB aabb(
		Vec3(-std::numeric_limits<float>::infinity()),
		Vec3(std::numeric_limits<float>::infinity()));
	_triangles = Octree<Triangle>(aabb);

	_backgroundColor = Rgb(0, 0, 0);

	std::ifstream file(path);
	if (!file.is_open())
		throw "file not found";
	std::string line;
	while (std::getline(file, line)) {
		if (is_comment(line))
			continue;
		std::vector<std::string> blocks = split(line, ' ');
		to_light(blocks, _lights);
		to_camera(blocks, _camera);
		to_sphere(blocks, _spheres);
		to_triangle(blocks, _triangles);
		if (env::read_resolution_from == Resolution_from::rt_file)
			to_resolution(blocks, resolution);
	}
	file.close();
	_triangles.shirk_to_fit();
	_triangles.subdivide();
}

std::ostream& operator<<(std::ostream& o, const Scene& scene) {
	o << "Scene:\n";
	o << "  resolution: " << scene.resolution.width() << "x" << scene.resolution.height() << "\n";
	o << "  pixels    : " << scene.resolution.width() * scene.resolution.height() << "\n";
	o << "  lights    : " << scene._lights.size() << "\n";
	o << "  spheres   : " << scene._spheres.size() << "\n";
	o << "  triangles : " << scene._triangles.size() << "\n";
	return o;
}

Hit Scene::hit(const Ray& ray) const {
	return _triangles.hit(ray);
}

bool Scene::is_clear_path(const Vec3& point, const Light& light) const {
	Vec3 v = light._origin - point;
	v.normalize();
	Ray toLight(point, v);

	Hit hit = Scene::hit(toLight);
	return !hit.hit || hit.dist * hit.dist > light._origin.distance2(point);
}

Rgb Scene::get_color(const Ray& ray) const {
	Hit hit = this->hit(ray);
	if (!hit.hit)
		return _backgroundColor;

	Vec3 lightAccumulator = Vec3(0);
	for (auto& light : _lights) {
		if (!is_clear_path(hit.point, light))
			continue;
		float relativeIntensity = light.relative_intensity(hit.point, hit.normal);
		lightAccumulator += light._color * relativeIntensity;
	}

	Rgb color(0);
	color += lightAccumulator.min(Vec3(255, 255, 255));
	return mix_color(color, hit.color);
}

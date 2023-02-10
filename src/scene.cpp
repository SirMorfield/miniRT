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

bool Scene::parse_line(const std::string& line) {
	std::vector<std::string> blocks = split(line, ' ');
	if (blocks.size() == 0)
		return true;

	std::optional<Sphere> sphere = to_sphere(blocks);
	if (sphere) {
		_spheres.push_back(*sphere);
		return true;
	}

	std::optional<Triangle> triangle = to_triangle(blocks);
	if (triangle) {
		_triangles.push_back(*triangle);
		return true;
	}

	std::optional<Light> light = to_light(blocks);
	if (light) {
		_lights.push_back(*light);
		return true;
	}

	std::optional<Camera> camera = to_camera(blocks);
	if (camera) {
		_camera = *camera;
		return true;
	}

	std::optional<Resolution> res = to_resolution(blocks);
	if (res) {
		if (env::read_resolution_from == Resolution_from::rt_file)
			resolution = *res;
		return true;
	}

	if (blocks.at(0) == "A" || blocks.at(0) == "pl") {
		return true;
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
		throw std::runtime_error("Could not open file: " + path);

	size_t		i = 0;
	std::string line;
	while (std::getline(file, line)) {
		if (is_comment(line))
			continue;
		if (!this->parse_line(line))
			throw std::runtime_error("\nCould not parse file: " + path + ":" + std::to_string(i) + "\n\"" + line + "\"");

		i++;
	}
	file.close();
	_triangles.shirk_to_fit();
	_triangles.subdivide();
}

std::ostream& operator<<(std::ostream& o, const Scene& scene) {
	o << "Scene:\n";
	o << "  lights    : " << std::scientific << (double)scene._lights.size() << "\n";
	o << "  spheres   : " << std::scientific << (double)scene._spheres.size() << "\n";
	o << "  triangles : " << std::scientific << (double)scene._triangles.size() << "\n";
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

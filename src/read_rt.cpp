#include "arithmetic.hpp"
#include "env.hpp"
#include "io.hpp"
#include "octree.hpp"
#include "shapes.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>

// expect eg. "23,254,22"
std::optional<Rgb> parse_Rgb(const std::string& s) {
	Vec<int, 3> color;
	if (sscanf(s.c_str(), "%d,%d,%d", &color.x(), &color.y(), &color.z()) == -1)
		return std::nullopt;

	if (color.x() < 0 || color.x() > 255)
		return std::nullopt;
	if (color.y() < 0 || color.y() > 255)
		return std::nullopt;
	if (color.z() < 0 || color.z() > 255)
		return std::nullopt;

	return Rgb(color.x(), color.y(), color.z());
}

std::optional<Vec3> parse_Vec3(const std::string& s, bool should_be_normalized = false) {
	Vec3 v;
	if (sscanf(s.c_str(), "%f,%f,%f", &v.x(), &v.y(), &v.z()) == -1)
		return std::nullopt;

	if (should_be_normalized && !almost_equal(v.length(), 1.0f))
		return std::nullopt;

	return v;
}

std::optional<Light> to_light(const std::vector<std::string>& blocks) {
	if (blocks.size() != 4)
		return std::nullopt;

	if (blocks.at(0) != "l")
		return std::nullopt;

	std::optional<Vec3> origin = parse_Vec3(blocks.at(1));
	if (!origin)
		return std::nullopt;

	std::optional<float> brightness = parse_number<float>(blocks.at(2));
	if (!brightness)
		return std::nullopt;

	std::optional<Rgb> color = parse_Rgb(blocks.at(3));
	if (!color)
		return std::nullopt;

	return Light(*color, *origin, *brightness);
}

std::optional<Sphere> to_sphere(const std::vector<std::string>& blocks) {
	if (blocks.size() != 4)
		return std::nullopt;

	if (blocks.at(0) != "sp")
		return std::nullopt;

	std::optional<Vec3> origin = parse_Vec3(blocks.at(1));
	if (!origin)
		return std::nullopt;

	std::optional<float> radius = parse_number<float>(blocks.at(2));
	if (!radius)
		return std::nullopt;

	std::optional<Rgb> color = parse_Rgb(blocks.at(3));
	if (!color)
		return std::nullopt;

	return Sphere(*origin, *color, *radius);
}

std::optional<Camera> to_camera(const std::vector<std::string>& blocks) {
	if (blocks.size() != 4)
		return std::nullopt;

	if (blocks.at(0) != "c")
		return std::nullopt;

	std::optional<Vec3> origin = parse_Vec3(blocks.at(1));
	if (!origin)
		return std::nullopt;

	std::optional<Vec3> dir = parse_Vec3(blocks.at(2), !env::allow_abnormal_dir);
	if (!dir)
		return std::nullopt;
	dir->normalize();

	std::optional<float> fov = parse_number<float>(blocks.at(3));
	if (!fov)
		return std::nullopt;

	return Camera(*origin, *dir, radians(*fov));
}

std::optional<Triangle> to_triangle(const std::vector<std::string>& blocks) {
	if (blocks.size() != 5)
		return std::nullopt;

	if (blocks.at(0) != "tr")
		return std::nullopt;
	std::optional<Vec3> p0 = parse_Vec3(blocks.at(1));
	if (!p0)
		return std::nullopt;
	std::optional<Vec3> p1 = parse_Vec3(blocks.at(2));
	if (!p1)
		return std::nullopt;
	std::optional<Vec3> p2 = parse_Vec3(blocks.at(3));
	if (!p2)
		return std::nullopt;
	std::optional<Rgb> color = parse_Rgb(blocks.at(4));
	if (!color)
		return std::nullopt;
	return Triangle(*color, *p0, *p1, *p2);
}

std::optional<Resolution> to_resolution(const std::vector<std::string>& blocks) {
	if (blocks.size() != 3)
		return std::nullopt;

	if (blocks.at(0) != "R")
		return std::nullopt;

	std::optional<size_t> width = parse_number<size_t>(blocks.at(1));
	if (!width || *width <= 2)
		return std::nullopt;

	std::optional<size_t> height = parse_number<size_t>(blocks.at(2));
	if (!height || *height <= 2)
		return std::nullopt;

	return Resolution(*width, *height, 1);
}

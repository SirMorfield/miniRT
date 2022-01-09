#include "io.hpp"
#include "shapes.hpp"
#include "types.hpp"
#include "util.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>

// expect eg. "23,254,22"
bool toRgb(Rgb& color, const std::string& s) {
	int r;
	int g;
	int b;
	if (sscanf(s.c_str(), "%d,%d,%d", &r, &g, &b) == -1)
		return false;
	color.r = r;
	color.g = g;
	color.b = b;
	return true;
}

bool toVec3(Vec3& p, const std::string& s) {
	float x;
	float y;
	float z;
	if (sscanf(s.c_str(), "%f,%f,%f", &x, &y, &z) == -1)
		return false;
	p.x = x;
	p.y = y;
	p.z = z;
	return true;
}

bool toFloat(float& f, const std::string& s) {
	try {
		f = std::stof(s);
		return true;
	} catch (const std::exception& e) {
		return false;
	}
}

void toLight(const std::vector<std::string>& blocks, std::vector<Light>& lights) {
	if (blocks.size() != 4)
		return;
	if (blocks.at(0) != "l")
		return;
	Vec3 origin;
	if (!toVec3(origin, blocks.at(1)))
		return;
	float brightness;
	if (!toFloat(brightness, blocks.at(2)))
		return;
	Rgb color;
	if (!toRgb(color, blocks.at(3)))
		return;
	lights.push_back(Light(color, origin, brightness));
}

void toSphere(const std::vector<std::string>& blocks, std::vector<Sphere>& spheres) {
	if (blocks.size() != 4)
		return;
	if (blocks.at(0) != "sp")
		return;
	Vec3 origin;
	if (!toVec3(origin, blocks.at(1)))
		return;
	float radius;
	if (!toFloat(radius, blocks.at(2)))
		return;
	Rgb color;
	if (!toRgb(color, blocks.at(3)))
		return;
	spheres.push_back(Sphere(origin, color, radius));
}

void toCamera(const std::vector<std::string>& blocks, Camera& camera) {
	if (blocks.size() != 4)
		return;
	if (blocks.at(0) != "c")
		return;
	Vec3 origin;
	if (!toVec3(origin, blocks.at(1)))
		return;
	Vec3 dir;
	if (!toVec3(dir, blocks.at(2)))
		return;
	float fov;
	if (!toFloat(fov, blocks.at(3)))
		return;
	camera = Camera(origin, dir, radians(fov));
}

void toTriangle(const std::vector<std::string>& blocks, std::vector<Triangle>& triangles) {
	if (blocks.size() != 5)
		return;
	if (blocks.at(0) != "tr")
		return;
	Vec3 p0;
	if (!toVec3(p0, blocks.at(1)))
		return;
	Vec3 p1;
	if (!toVec3(p1, blocks.at(2)))
		return;
	Vec3 p2;
	if (!toVec3(p2, blocks.at(3)))
		return;
	Rgb color;
	if (!toRgb(color, blocks.at(4)))
		return;
	triangles.push_back(Triangle(color, p0, p1, p2));
}

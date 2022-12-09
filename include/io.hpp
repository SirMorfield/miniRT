#pragma once
#include "octree.hpp"
#include "shapes.hpp"
#include "types.hpp"

bool					  save_bmp(size_t xSize, size_t ySize, const std::vector<Rgb>& frame, const std::string path);

std::optional<Light>	  to_light(const std::vector<std::string>& blocks);
std::optional<Sphere>	  to_sphere(const std::vector<std::string>& blocks);
std::optional<Camera>	  to_camera(const std::vector<std::string>& blocks);
std::optional<Triangle>	  to_triangle(const std::vector<std::string>& blocks);
std::optional<Resolution> to_resolution(const std::vector<std::string>& blocks);

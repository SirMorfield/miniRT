#pragma once
#include "shapes.hpp"
#include "types.hpp"

bool save_bmp(size_t xSize, size_t ySize, const std::vector<Rgb>& frame, const std::string path);
void to_light(const std::vector<std::string>& blocks, std::vector<Light>& lights);
void to_camera(const std::vector<std::string>& blocks, Camera& camera);
void to_sphere(const std::vector<std::string>& blocks, std::vector<Sphere>& spheres);
void to_triangle(const std::vector<std::string>& blocks, std::vector<Triangle>& triangles);

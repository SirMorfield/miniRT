#pragma once
#include "shapes.hpp"
#include "types.hpp"

bool save_bmp(size_t xSize, size_t ySize, const std::vector<Rgb>& frame, const std::string path);
void toLight(const std::vector<std::string>& blocks, std::vector<Light>& lights);
void toCamera(const std::vector<std::string>& blocks, Camera& camera);
void toSphere(const std::vector<std::string>& blocks, std::vector<Sphere>& spheres);
void toTriangle(const std::vector<std::string>& blocks, std::vector<Triangle>& triangles);

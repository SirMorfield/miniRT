#pragma once
#include "lighting.hpp"
#include "shapes.hpp"
#include "types.hpp"
#include "util.hpp"
#include <set>
#include <vector>

class Scene {
  public:
	Scene();
	Scene(const std::string& path);
	Hit	   hit(const Ray& ray, const std::set<ID>& ignore) const;
	Rgb	   getColor(const Ray& ray) const;
	Camera _camera;

  private:
	bool				isClearPath(const std::set<ID>& ignore, const Vec3& point, const Light& light) const;
	std::vector<Sphere> _spheres;
	std::vector<Light>	_lights;
	Rgb					_backgroundColor;
};

class Renderer {
  public:
	Renderer(size_t xSize, size_t ySize);
	void render(const Scene& scene);
	void saveToBMP();

  private:
	Ray				 _rayFromPixel(const Camera& camera, float x, float y);
	size_t			 _xSize;
	size_t			 _ySize;
	float			 _aspectRatio;
	std::vector<Rgb> _frame;
};

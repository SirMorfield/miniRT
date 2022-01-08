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
	Hit hit(const Ray& ray, const std::set<ID>& ignore) const;
	Rgb getColor(const Ray& ray) const;

  private:
	bool				isClearPath(const std::set<ID>& ignore, const Vec3& point, const Light& light) const;
	std::vector<Sphere> _spheres;
	std::vector<Light>	_lights;
	Rgb					_backgroundColor;
};

class Camera {
  public:
	Camera() {}
	Camera(Vec3 pos, Vec3 dir, float fov);

	//   private:
	Vec3  _pos;
	Vec3  _dir;
	float _fov;
};

class Renderer {
  public:
	Renderer(size_t xSize, size_t ySize);
	void render(const Scene& scene);
	void saveToBMP();

  private:
	Ray				 _rayFromPixel(float x, float y);
	size_t			 _xSize;
	size_t			 _ySize;
	Camera			 _camera;
	float			 _aspectRatio;
	float			 _fov;
	std::vector<Rgb> _frame;
};

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
	bool				  isClearPath(const std::set<ID>& ignore, const Vec3& point, const Light& light) const;
	std::vector<Sphere>	  _spheres;
	std::vector<Triangle> _triangles;
	std::vector<Light>	  _lights;
	Rgb					  _backgroundColor;
};

template <class T>
Hit hitShape(const std::vector<T>& objects, const Ray& ray, const std::set<ID>& ignore) {
	for (typename std::vector<const T>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if (ignore.find(it->id) != ignore.end())
			continue;
		Intersect intersect = it->intersect(ray);
		if (intersect.hit)
			return Hit(intersect.dist, intersect.point, intersect.normal, it->color, it->id);
	}
	return Hit(false);
}

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

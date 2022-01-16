#pragma once
#include "lighting.hpp"
#include "shapes.hpp"
#include "types.hpp"
#include "util.hpp"
#include <set>
#include <thread>
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
	for (auto& object : objects) {
		if (ignore.find(object.id) != ignore.end())
			continue;
		Intersect intersect = object.intersect(ray);
		if (intersect.hit)
			return Hit(intersect.dist, intersect.point, intersect.normal, object.color, object.id);
	}
	return Hit(false);
}

class FrameBuffer {
  public:
	FrameBuffer(size_t xSize, size_t ySize);
	bool getPixel(size_t& x, size_t& y);
	void setPixel(const Rgb& color, size_t x, size_t y);
	void saveToBMP();

  private:
	size_t			 _maxI;
	size_t			 _xSize;
	size_t			 _ySize;
	size_t			 _i;
	std::mutex		 _mutex;
	std::vector<Rgb> _frame;

	// disabled
	FrameBuffer();
};

class Renderer {
  public:
	Renderer(size_t xSize, size_t ySize);
	void thread(const Scene& scene, FrameBuffer* fb);

  private:
	Ray	   _rayFromPixel(const Camera& camera, float x, float y) const;
	size_t _xSize;
	size_t _ySize;
	float  _aspectRatio;
};

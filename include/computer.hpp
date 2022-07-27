#pragma once
#include "lighting.hpp"
#include "shapes.hpp"
#include "types.hpp"
#include "util.hpp"
#include <mutex>
#include <set>
#include <thread>
#include <vector>

class Scene {
  public:
	Scene();
	Scene(const std::string& path);
	Hit		   hit(const Ray& ray, const std::set<ID>& ignore) const;
	Rgb		   get_color(const Ray& ray) const;
	Camera	   _camera;
	Resolution resolution;

  private:
	bool				  is_clear_path(const std::set<ID>& ignore, const Vec3& point, const Light& light) const;
	std::vector<Sphere>	  _spheres;
	std::vector<Triangle> _triangles;
	std::vector<Light>	  _lights;
	Rgb					  _backgroundColor;

	friend std::ostream&  operator<<(std::ostream& o, const Scene& scene);
};

std::ostream& operator<<(std::ostream& o, const Scene& scene);

template <class T>
Hit hit_shape(const std::vector<T>& objects, const Ray& ray, const std::set<ID>& ignore) {
	for (auto& object : objects) {
		if (ignore.find(object.id) != ignore.end())
			continue;
		Intersect intersect = object.intersect(ray);
		if (intersect.hit)
			return Hit(intersect.dist, intersect.point, intersect.normal, object.color, object.id);
	}
	return Hit(false);
}

class Frame_buffer {
  public:
	Frame_buffer(size_t xSize, size_t ySize);
	bool get_pixel(size_t& x, size_t& y);
	void set_pixel(const Rgb& color, size_t x, size_t y);
	void save_to_BMP();

  private:
	size_t			 _max_i;
	size_t			 _x_size;
	size_t			 _y_size;
	size_t			 _i;
	std::mutex		 _mutex;
	std::vector<Rgb> _frame;

	// disabled
	Frame_buffer();
};

class Renderer {
  public:
	Renderer(size_t xSize, size_t ySize);
	void thread(const Scene& scene, Frame_buffer* fb);

  private:
	Ray	   ray_from_pixel(const Camera& camera, float x, float y) const;
	size_t _x_size;
	size_t _y_size;
	float  _aspect_ratio;
};

#pragma once
#include "lighting.hpp"
#include "octree.hpp"
#include "shapes.hpp"
#include "types.hpp"
#include "util.hpp"
#include <mutex>
#include <thread>
#include <vector>

class Scene {
  public:
	Scene();
	Scene(const std::string& path);
	Hit		   hit(const Ray& ray) const;
	Rgb		   get_color(const Ray& ray) const;
	Camera	   _camera;
	Resolution resolution;

  private:
	bool				 is_clear_path(const Vec3& point, const Light& light) const;
	std::vector<Sphere>	 _spheres;
	Octree<Triangle>	 _triangles;
	std::vector<Light>	 _lights;
	Rgb					 _backgroundColor;

	friend std::ostream& operator<<(std::ostream& o, const Scene& scene);
};

std::ostream& operator<<(std::ostream& o, const Scene& scene);

class Frame_buffer {
  public:
	Frame_buffer(size_t xSize, size_t ySize);
	std::optional<Point2<size_t>> get_pixel();
	void						  set_pixel(const Rgb& color, size_t x, size_t y);
	void						  save_to_BMP() const;

  private:
	size_t			 _max_i;
	size_t			 _x_size;
	size_t			 _y_size;
	size_t			 _i;
	std::mutex		 _mutex;
	std::vector<Rgb> _frame;
	Progress_logger	 _progress;

	// disabled
	Frame_buffer();
};

class Renderer {
  public:
	Renderer(size_t xSize, size_t ySize, size_t AA_level);
	void thread(const Scene& scene, Frame_buffer* fb);

  private:
	Ray	   ray_from_pixel(const Camera& camera, float x, float y) const;
	size_t _x_size;
	size_t _y_size;
	size_t _AA_level;
	float  _aspect_ratio;
};

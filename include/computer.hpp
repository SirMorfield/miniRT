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
	bool				 parse_line(const std::string& line);
	std::vector<Sphere>	 _spheres;
	Octree<Triangle>	 _triangles;
	std::vector<Light>	 _lights;
	Rgb					 _backgroundColor;

	friend std::ostream& operator<<(std::ostream& o, const Scene& scene);
};

std::ostream& operator<<(std::ostream& o, const Scene& scene);

class Random_counter {
  public:
	Random_counter(size_t max);
	std::optional<size_t> next();
	size_t				  get() const;
	void				  reset();

  private:
	size_t		 _i;
	size_t		 _offset;
	const size_t _change;
	size_t		 _done;
	const size_t _max;
};

class Frame_buffer {
  public:
	Frame_buffer(const Resolution& resolution, bool log_progress = false);
	std::optional<Vec<size_t, 2>> get_pixel();
	void						  set_pixel(const Rgb& color, size_t x, size_t y);
	void						  save_to_BMP() const;
	size_t						  max_i() const { return _resolution.width() * _resolution.height(); }

  private:
	const Resolution _resolution;
	const bool		 _log_progress;

	Random_counter	 _i;
	size_t			 _pix_done;

	std::vector<Rgb> _frame;
	std::mutex		 _mutex;
	Progress_logger	 _progress;

	// disabled
	Frame_buffer();
};

class Renderer {
  public:
	Renderer(const Resolution& resolution);
	void thread(const Scene& scene, Frame_buffer* fb);

  private:
	Ray		   ray_from_pixel(const Camera& camera, float x, float y) const;
	Resolution _resolution;
	float	   _aspect_ratio;
};

#include "computer.hpp"
#include <cmath>

Camera::Camera(Vec3 pos, Vec3 dir, float fov)
	: _pos(pos),
	  _dir(dir),
	  _fov(fov),
	  _fov_tan(tan(fov * 0.5)) {
	_dir.normalize();
}

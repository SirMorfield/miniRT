#include "main.hpp"
Hit Sphere::hit(const Vec3& origin, const Vec3& dir) {
	(void)origin;
	(void)dir;
	Hit hit(false, 999.0f, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	return hit;
}

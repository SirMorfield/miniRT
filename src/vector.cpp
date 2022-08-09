#include "vector.hpp"
#include "util.hpp"
#include <cmath>

// TODO: look into
// typedef float	vec3f __attribute__((vector_size(sizeof(float) * 4)));

Vec3 correct_normal(const Vec3& normal, const Ray& ray) {
	const Vec3 inverse = normal * -1.0f;
	if (normal.dot(ray.dir) < inverse.dot(ray.dir))
		return inverse;
	else
		return normal;
}

Vec3::Vec3() {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
Vec3::Vec3(float xyz) : Vec3(xyz, xyz, xyz) {}

Vec3& Vec3::operator=(const Vec3& cp) {
	this->x = cp.x;
	this->y = cp.y;
	this->z = cp.z;
	return *this;
}

Vec3::Vec3(const Vec3& cp) { *this = cp; }

Vec3 Vec3::operator+(const Vec3& cp) const { return Vec3(x + cp.x, y + cp.y, z + cp.z); }
Vec3 Vec3::operator-(const Vec3& cp) const { return Vec3(x - cp.x, y - cp.y, z - cp.z); }
Vec3 Vec3::operator*(const Vec3& cp) const { return Vec3(x * cp.x, y * cp.y, z * cp.z); }
Vec3 Vec3::operator/(const Vec3& cp) const { return Vec3(x / cp.x, y / cp.y, z / cp.z); }

Vec3 Vec3::operator+(float f) const { return Vec3(x + f, y + f, z + f); }
Vec3 Vec3::operator-(float f) const { return Vec3(x - f, y - f, z - f); }
Vec3 Vec3::operator*(float f) const { return Vec3(x * f, y * f, z * f); }
Vec3 Vec3::operator/(float f) const { return Vec3(x / f, y / f, z / f); }

Vec3	   Vec3::cross(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
float	   Vec3::dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

//
void Vec3::normalize() {
	float invLen = 1 / std::sqrt(x * x + y * y + z * z);
	x *= invLen;
	y *= invLen;
	z *= invLen;
}

float Vec3::length2() const { return x * x + y * y + z * z; }
float Vec3::length() const { return std::sqrt(length2()); }

float Vec3::distance2(const Vec3& v) const {
	Vec3 ab = *this - v;
	return ab.x * ab.x + ab.y * ab.y + ab.z * ab.z;
}
float Vec3::distance(const Vec3& v) const { return std::sqrt(distance2(v)); }

//
void Vec3::translate(const Vec3& dir, float t) {
	x += dir.x * t;
	y += dir.y * t;
	z += dir.z * t;
}

std::ostream& operator<<(std::ostream& o, const Vec3& v) {
	o << v.x << ", " << v.y << ", " << v.z << " ";
	return o;
}

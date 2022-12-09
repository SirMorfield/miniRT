#pragma once
#include <cmath>
#include <iostream>

template <class T>
class Vec3x {
  public:
	Vec3x(){};
	Vec3x(T x, T y, T z) : x(x), y(y), z(z){};
	Vec3x(T xyz) : Vec3x(xyz, xyz, xyz){};
	Vec3x(const Vec3x& cp) {
		*this = cp;
	}

	Vec3x<T>& operator=(const Vec3x<T>& cp) {
		this->x = cp.x;
		this->y = cp.y;
		this->z = cp.z;
		return *this;
	}

	Vec3x<T> operator+(const Vec3x& cp) const { return Vec3x(x + cp.x, y + cp.y, z + cp.z); }
	Vec3x<T> operator-(const Vec3x& cp) const { return Vec3x(x - cp.x, y - cp.y, z - cp.z); }
	Vec3x<T> operator*(const Vec3x& cp) const { return Vec3x(x * cp.x, y * cp.y, z * cp.z); }
	Vec3x<T> operator/(const Vec3x& cp) const { return Vec3x(x / cp.x, y / cp.y, z / cp.z); }

	Vec3x<T> operator+(float f) const { return Vec3x(x + f, y + f, z + f); }
	Vec3x<T> operator-(float f) const { return Vec3x(x - f, y - f, z - f); }
	Vec3x<T> operator*(float f) const { return Vec3x(x * f, y * f, z * f); }
	Vec3x<T> operator/(float f) const { return Vec3x(x / f, y / f, z / f); }

	Vec3x<T> operator+=(const Vec3x& cp) { return *this = *this + cp; }
	Vec3x<T> operator-=(const Vec3x& cp) { return *this = *this - cp; }
	Vec3x<T> operator*=(const Vec3x& cp) { return *this = *this * cp; }
	Vec3x<T> operator/=(const Vec3x& cp) { return *this = *this / cp; }

	Vec3x<T> operator+=(float f) { return *this = *this + f; }
	Vec3x<T> operator-=(float f) { return *this = *this - f; }
	Vec3x<T> operator*=(float f) { return *this = *this * f; }
	Vec3x<T> operator/=(float f) { return *this = *this / f; }

	//
	T length2() const {
		return x * x + y * y + z * z;
	}
	T length() const {
		return std::sqrt(length2());
	}
	void normalize() {
		float invLen = 1 / std::sqrt(x * x + y * y + z * z);
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}
	void translate(const Vec3x<T>& dir, T t) {
		x += dir.x * t;
		y += dir.y * t;
		z += dir.z * t;
	}

	Vec3x<T> cross(const Vec3x<T>& v) const {
		return Vec3x(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	T dot(const Vec3x<T>& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	T distance2(const Vec3x<T>& v) const {
		Vec3x ab = *this - v;
		return ab.x * ab.x + ab.y * ab.y + ab.z * ab.z;
	}
	T distance(const Vec3x<T>& v) const {
		return std::sqrt(distance2(v));
	}

	T x;
	T y;
	T z;
};

typedef Vec3x<float> Vec3;

template <class T>
std::ostream& operator<<(std::ostream& o, const Vec3x<T>& v) {
	o << v.x << ", " << v.y << ", " << v.z << " ";
	return o;
}

class Ray {
  public:
	Ray() {}
	Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir) {}
	Vec3 origin;
	Vec3 dir;
};

template <class T>
Vec3x<T> correct_normal(const Vec3x<T>& normal, const Ray& ray) {
	const Vec3 inverse = normal * -1.0f;
	if (normal.dot(ray.dir) < inverse.dot(ray.dir))
		return inverse;
	else
		return normal;
}

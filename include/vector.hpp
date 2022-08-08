#pragma once
#include <iostream>

class Vec3 {
  public:
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(float xyz);
	Vec3& operator=(const Vec3& cp);
	Vec3(const Vec3& cp);
	~Vec3();

	Vec3  operator+(const Vec3& cp) const;
	Vec3  operator-(const Vec3& cp) const;
	Vec3  operator*(const Vec3& cp) const;
	Vec3  operator/(const Vec3& cp) const;

	Vec3  operator+(float f) const;
	Vec3  operator-(float f) const;
	Vec3  operator*(float f) const;
	Vec3  operator/(float f) const;

	float length2() const;
	float length() const;
	void  normalize();
	void  translate(const Vec3& dir, float t);
	Vec3  cross(const Vec3& v) const;
	float dot(const Vec3& v) const;
	float distance2(const Vec3& v) const;
	float distance(const Vec3& v) const;

	float x;
	float y;
	float z;
};
std::ostream& operator<<(std::ostream& o, const Vec3& v);

class Ray;
Vec3 correct_normal(const Vec3& normal, const Ray& ray);

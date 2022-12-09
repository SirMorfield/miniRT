#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>

template <typename T, size_t N>
class Vec {
  public:
	Vec() {}
	Vec(std::array<T, N> data) {
		assert(data.size() == N);
		for (size_t i = 0; i < N; i++) {
			d[i] = data[i];
		}
	};
	Vec(const T& value) {
		for (T& i : d)
			i = value;
	}
	// typename std::enable_if<(N == 2), Vec<T, 3>>::type
	Vec(const T& x, const T& y) {
		static_assert(N == 2);
		d[0] = x;
		d[1] = y;
	}
	// typename std::enable_if<(N == 3), Vec<T, 3>>::type
	Vec(const T& x, const T& y, const T& z) {
		static_assert(N == 3);
		d[0] = x;
		d[1] = y;
		d[2] = z;
	}

	template <typename T2>
	Vec<T, N>& operator=(const Vec<T2, N>& v) {
		for (size_t i = 0; i < N; i++) {
			d[i] = v.d[i];
		}
		return *this;
	}

	template <typename T2>
	Vec<T, N> operator+(const Vec<T2, N>& v) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] + v.d[i];
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator-(const Vec<T2, N>& v) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] - v.d[i];
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator*(const Vec<T2, N>& v) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] * v.d[i];
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator/(const Vec<T2, N>& v) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] / v.d[i];
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator+(T2 f) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] + f;
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator-(T2 f) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] - f;
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator*(T2 f) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] * f;
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator/(T2 f) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] / f;
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> operator+=(const Vec<T2, N>& v) { return *this = *this + v; }
	template <typename T2>
	Vec<T, N> operator-=(const Vec<T2, N>& v) { return *this = *this - v; }
	template <typename T2>
	Vec<T, N> operator*=(const Vec<T2, N>& v) { return *this = *this * v; }
	template <typename T2>
	Vec<T, N> operator/=(const Vec<T2, N>& v) { return *this = *this / v; }

	template <typename T2>
	Vec<T, N> operator+=(T2 f) { return *this = *this + f; }
	template <typename T2>
	Vec<T, N> operator-=(T2 f) { return *this = *this - f; }
	template <typename T2>
	Vec<T, N> operator*=(T2 f) { return *this = *this * f; }
	template <typename T2>
	Vec<T, N> operator/=(T2 f) { return *this = *this / f; }

	//
	T& operator[](size_t i) {
		assert(i < N);
		return d[i];
	}
	const T& operator[](size_t i) const {
		assert(i < N);
		return d[i];
	}

	template <typename T2>
	T dot(const Vec<T2, N>& v) const {
		T res = 0;
		for (size_t i = 0; i < N; i++) {
			res += d[i] * v.d[i];
		}
		return res;
	}

	T length2() const {
		T res = 0;
		for (size_t i = 0; i < N; i++) {
			res += d[i] * d[i];
		}
		return res;
	}

	T length() const {
		return std::sqrt(length2());
	}

	void normalize() {
		T len = length();
		for (size_t i = 0; i < N; i++) {
			d[i] /= len;
		}
	}

	[[nodiscard]] Vec<T, N> normalized() const {
		Vec<T, N> res = *this;
		res.normalize();
		return res;
	}

	template <typename T2>
	void translate(const Vec<T2, N>& dir, T t) {
		for (size_t i = 0; i < N; i++) {
			d[i] += dir.d[i] * t;
		}
	}

	template <typename T2>
	T distance2(const Vec<T2, N>& v) const {
		T dist = 0;
		for (size_t i = 0; i < N; i++) {
			dist += std::pow(d[i] - v.d[i], 2);
		}
		return dist;
	}

	template <typename T2>
	T distance(const Vec<T2, N>& v) const {
		return std::sqrt(distance2(v));
	}

	template <typename T2>
	Vec<T, N> min(const Vec<T2, N>& v) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] < v.d[i] ? d[i] : v.d[i];
		}
		return res;
	}

	template <typename T2>
	Vec<T, N> max(const Vec<T2, N>& v) const {
		Vec<T, N> res;
		for (size_t i = 0; i < N; i++) {
			res.d[i] = d[i] > v.d[i] ? d[i] : v.d[i];
		}
		return res;
	}

	template <typename T2>
	// typename std::enable_if<(N == 3), Vec<T, 3>>::type
	Vec<T, 3> cross(const Vec<T2, 3>& v) const {
		return Vec(d[1] * v.d[2] - d[2] * v.d[1],
				   d[2] * v.d[0] - d[0] * v.d[2],
				   d[0] * v.d[1] - d[1] * v.d[0]);
	}

	// typename std::enable_if<(N >= 1), T&>::type
	T& x() {
		static_assert(N >= 1);
		return d[0];
	}

	// typename std::enable_if<(N >= 2), const T&>::type
	T& y() {
		static_assert(N >= 2);
		return d[1];
	}

	// typename std::enable_if<(N >= 3), const T&>::type
	T& z() {
		static_assert(N >= 3);
		return d[2];
	}

	const T& x() const {
		static_assert(N >= 1);
		return d[0];
	}

	const T& y() const {
		static_assert(N >= 2);
		return d[1];
	}

	const T& z() const {
		static_assert(N >= 3);
		return d[2];
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec<T, N>& v) {
		// for (size_t i = 0; i < N; i++) {
		// 	os << v.d[i];
		// 	if (i + 1 < N) {
		// 		os << ", ";
		// 	}
		// }
		(void)v;
		return os;
	}

	std::array<T, N> d;
};

typedef Vec<float, 3>	Vec3;
typedef Vec<uint8_t, 3> Rgb;

class Ray {
  public:
	Ray() {}
	Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir) {}
	Vec3 origin;
	Vec3 dir;
};

template <typename T, size_t N>
Vec<T, N> correct_normal(const Vec<T, N>& normal, const Ray& ray) {
	const Vec3 inverse = normal * -1.0f;
	if (normal.dot(ray.dir) < inverse.dot(ray.dir))
		return inverse;
	else
		return normal;
}

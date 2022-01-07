// this will be removed
#include "vector.hpp"

void normalize(Vec3* v) {
	float inv_len = 1 / std::sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x *= inv_len;
	v->y *= inv_len;
	v->z *= inv_len;
}

Vec3 unit(Vec3 v) {
	normalize(&v);
	return (v);
}

// = a - b || to, from
Vec3 subtract(Vec3 a, Vec3 b) {
	Vec3 v;

	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	return (v);
}

Vec3 scale(Vec3 v, float r) {
	Vec3 vec;

	vec.x = v.x * r;
	vec.y = v.y * r;
	vec.z = v.z * r;
	return (vec);
}

// Get point t distance away from origin in direction of (normalized) dir

Vec3 translate(Vec3 origin, Vec3 dir, float t) {
	return (add(origin, scale(dir, t)));
}

// The (always positive) squared distance between a and b

float distance2(Vec3 a, Vec3 b) {
	Vec3 ab;

	ab = subtract(a, b);
	return (ab.x * ab.x + ab.y * ab.y + ab.z * ab.z);
}

// The (always positive) distance between a and b

float distance(Vec3 a, Vec3 b) {
	return (sqrt(distance2(a, b)));
}

float length(Vec3 v) {
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

// @return product of the Euclidean magnitudes of the two vectors
// and the cosine of the angle between them.
// = length(a) * length(b) * cos(alpha)

float dot(Vec3 a, Vec3 b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// @return vector orthogonal (perpendicular) to a and b

Vec3 cross(Vec3 a, Vec3 b) {
	Vec3 v;

	v.x = a.y * b.z - a.z * b.y;
	v.y = a.z * b.x - a.x * b.z;
	v.z = a.x * b.y - a.y * b.x;
	return (v);
}

Vec3 add(Vec3 a, Vec3 b) {
	Vec3 v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return (v);
}

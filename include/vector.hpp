#pragma once
#include "types.hpp"
class Vec3;
void  normalize(Vec3* v);
Vec3  unit(Vec3 v);
Vec3  subtract(Vec3 a, Vec3 b);
Vec3  scale(Vec3 v, float r);
Vec3  translate(Vec3 origin, Vec3 dir, float t);
float distance2(Vec3 a, Vec3 b);
float distance(Vec3 a, Vec3 b);
float length(Vec3 v);
float dot(Vec3 a, Vec3 b);
Vec3  cross(Vec3 a, Vec3 b);
Vec3  add(Vec3 a, Vec3 b);

#pragma once
#include "shapes.hpp"
#include "types.hpp"
#include <set>

template <class T>
Hit hit_shape(const std::vector<T>& objects, const Ray& ray, const std::set<ID>& ignore) {
	Hit closest_hit(false);

	for (auto& object : objects) {
		if (ignore.find(object.id) != ignore.end())
			continue;
		Intersect intersect = object.intersect(ray);
		if (intersect.hit && (!closest_hit.hit || intersect.dist < closest_hit.dist))
			closest_hit = Hit(intersect.dist, intersect.point, intersect.normal, object.color, object.id);
	}
	return closest_hit;
}

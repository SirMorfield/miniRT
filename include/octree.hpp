#pragma once
#include "shapes.hpp"
#include <limits>
#define N_SUBNODES 8
#define MAX_ELEMENTS_PER_NODE 10

// TODO: template for data storage

template <class T>
class Octree {
  public:
	Octree() : Octree(AABB(Vec3(0, 0, 0), Vec3(0, 0, 0))) {}

	Octree(const AABB& aabb)
		: _size(0),
		  _aabb(aabb) {}

	void subdivide() {
		if (this->_children.empty()) {
			std::vector<AABB> sub_aabbs = this->_aabb.subdivide();
			this->_children.reserve(sub_aabbs.size());
			for (const AABB& aabb : sub_aabbs) {
				this->_children.push_back(Octree<T>(aabb));
			}
		}

		this->_shapes.erase(
			std::remove_if(this->_shapes.begin(), this->_shapes.end(), [&](const auto& shape) {
				for (Octree<T>& child : this->_children) {
					if (child.insert(shape))
						return true;
				}
				return false;
			}),
			this->_shapes.end());
	}

	[[nodiscard]] bool insert(const T& shape) {
		if (!shape.is_inside_AABB(this->_aabb))
			return false;
		_size++;

		for (auto& child : this->_children) {
			if (shape.is_inside_AABB(child._aabb)) {
				assert(child.insert(shape));
				return true;
			}
		}

		if (this->_shapes.size() + 1 > MAX_ELEMENTS_PER_NODE) {
			this->subdivide();
		}

		this->_shapes.push_back(shape);
		return true;
	}

	// TODO: first check children, if hit: ignore subdivisions behind the object
	Hit hit(const Ray& ray) const {
		if (!this->_aabb.intersect(ray))
			return Hit(false);

		// find closest hit in all of the children
		Hit closest_hit(false);
		for (const Octree<T>& child : this->_children) {
			Hit hit = child.hit(ray);
			if (hit.hit && (!closest_hit.hit || hit.dist < closest_hit.dist))
				closest_hit = hit;
		}

		for (auto& shape : this->_shapes) {
			Hit hit = shape.hit(ray);
			if (hit.hit && (!closest_hit.hit || hit.dist < closest_hit.dist))
				closest_hit = hit;
		}
		return closest_hit;
	}

	void shirk_to_fit() {
		if (this->_shapes.size() == 0)
			return;

		Vec3 min = this->_aabb._min;
		Vec3 max = this->_aabb._max;

		for (const T& shape : this->_shapes) {
			const AABB& shape_AABB = shape.get_AABB();

			min.x() = std::max(min.x(), shape_AABB._min.x());
			min.y() = std::max(min.y(), shape_AABB._min.y());
			min.z() = std::max(min.z(), shape_AABB._min.z());

			max.x() = std::min(max.x(), shape_AABB._max.x());
			max.y() = std::min(max.y(), shape_AABB._max.y());
			max.z() = std::min(max.z(), shape_AABB._max.z());
		}

		this->_aabb = AABB(max, min);
	}

	void push_back(const T& shape) {
		this->_shapes.push_back(shape);
		this->_size++;
	}
	size_t size() const { return this->_size; }
	// Octree(const& Octree<T> other) = delete;
	// Octree& operator=(const& Octree<T> other) = delete;

  private:
	size_t				_size;
	AABB				_aabb;
	std::vector<T>		_shapes;
	std::vector<Octree> _children;
};

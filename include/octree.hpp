#pragma once
#include "shapes.hpp"
#include <limits>
#define N_SUBNODES 8
#define MAX_ELEMENTS_PER_NODE 1

// TODO: template for data storage

template <class T>
class Octree {
  public:
	Octree() : Octree(AABB(Vec3(0, 0, 0), Vec3(0, 0, 0))) {}

	Octree(const AABB& aabb) : _aabb(aabb) {
		_size = 0;
	}

	void subdivide() {
		if (this->_children.empty()) {
			std::vector<AABB> sub_aabbs = this->_aabb.subdivide();
			// this->_children.reserve(sub_aabbs.size());
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

	bool insert(const T& shape) {
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
	// void push_back(const T& shape) { this->insert(shape); }

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
			Intersect intersect = shape.intersect(ray);
			if (intersect.hit && (!closest_hit.hit || intersect.dist < closest_hit.dist))
				closest_hit = Hit(intersect.dist, intersect.point, intersect.normal, shape.color, shape.id);
		}
		return closest_hit;
	}

	void shirk_to_fit() {
		if (!this->is_leaf())
			return; // TODO: go all the way down the tree instead of stopping here
		if (this->_shapes.size() == 0)
			return;

		Vec3 min = this->_shapes[0]._min;
		Vec3 max = this->_shapes[0]._max;

		for (T& shape : this->_shapes) {
			Vec3& shape_AABB = shape.get_AABB();

			min.x = std::min(min.x, shape_AABB.x);
			min.y = std::min(min.y, shape_AABB.y);
			min.z = std::min(min.z, shape_AABB.z);

			max.x = std::max(max.x, shape_AABB.x);
			max.y = std::max(max.y, shape_AABB.y);
			max.z = std::max(max.z, shape_AABB.z);
		}
	}

	size_t size() const { return this->_size; }
	// Octree(const& Octree<T> other) = delete;
	// Octree& operator=(const& Octree<T> other) = delete;

  private:
	size_t				_size;
	AABB				_aabb;
	size_t				_max_elements_per_node;
	std::vector<T>		_shapes;
	std::vector<Octree> _children;
};

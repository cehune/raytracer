#include "acceleration/bvh_util.h"

BVHPrimitive::BVHPrimitive() {}

BVHPrimitive::BVHPrimitive(size_t primitiveIndex, const Bounds3f &bounds, std::shared_ptr<hittable> obj)
    : primitiveIndex(primitiveIndex), bounds(bounds), object(obj) {}

vec3h BVHPrimitive::Centroid() const {
    return 0.5 * bounds.pmin + 0.5 * bounds.pmax;
}

BVHTreeNode::BVHTreeNode(BVHTreeNode&& other) noexcept
    : left(std::move(other.left)),
      right(std::move(other.right)),
      bounds(std::move(other.bounds)),
      prims(std::move(other.prims)) {}

BVHTreeNode& BVHTreeNode::operator=(BVHTreeNode&& other) noexcept {
    if (this != &other) {
        left = std::move(other.left);
        right = std::move(other.right);
        bounds = std::move(other.bounds);
        prims = std::move(other.prims);
    }
    return *this;
}

BVHTreeNode::BVHTreeNode() = default;

bool BVHTreeNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

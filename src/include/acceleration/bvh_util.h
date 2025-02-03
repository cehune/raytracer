#ifndef BVH_UTIL_H
#define BVH_UTIL_H

#include <iostream>
#include <vector>
#include "../geometry/bounds.h"

struct BVHPrimitive {
    BVHPrimitive() {}
    BVHPrimitive(size_t primitiveIndex, const Bounds3f &bounds, shared_ptr<hittable> obj)
        : primitiveIndex(primitiveIndex), bounds(bounds), object(obj) {}
    size_t primitiveIndex;
    Bounds3f bounds;
    shared_ptr<hittable> object;
    // BVHPrimitive Public Methods
    vec3h Centroid() const { return .5f * bounds.pmin + .5f * bounds.pmax; }
};

struct BVHBucket {
    int num_prims = 0;
    Bounds3f bounds;
};

struct BVHTreeNode {
    std::unique_ptr<BVHTreeNode> left;
    std::unique_ptr<BVHTreeNode> right;
    Bounds3f bounds;
    std::vector<BVHPrimitive> prims;

    // Move constructor
    BVHTreeNode(BVHTreeNode&& other) noexcept
        : left(std::move(other.left)),
          right(std::move(other.right)),
          bounds(std::move(other.bounds)),
          prims(std::move(other.prims)) {}

    // Move assignment operator
    BVHTreeNode& operator=(BVHTreeNode&& other) noexcept {
        if (this != &other) {
            left = std::move(other.left);
            right = std::move(other.right);
            bounds = std::move(other.bounds);
            prims = std::move(other.prims);
        }
        return *this;
    }

    // Delete copy constructor and copy assignment operator (if not already deleted)
    BVHTreeNode() = default;
    BVHTreeNode(const BVHTreeNode&) = delete;
    BVHTreeNode& operator=(const BVHTreeNode&) = delete;

    // Check if the node is a leaf node
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};


#endif
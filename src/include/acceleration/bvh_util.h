// bvh_util.h
#ifndef BVH_UTIL_H
#define BVH_UTIL_H

#include <iostream>
#include <vector>
#include <memory>
#include "../geometry/bounds.h"
#include "../primitive_shapes/hittable.h"

struct BVHPrimitive {
    BVHPrimitive();
    BVHPrimitive(size_t primitiveIndex, const Bounds3f &bounds, std::shared_ptr<hittable> obj);

    size_t primitiveIndex;
    Bounds3f bounds;
    std::shared_ptr<hittable> object;

    vec3h Centroid() const;
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

    BVHTreeNode();
    BVHTreeNode(BVHTreeNode&& other) noexcept;
    BVHTreeNode& operator=(BVHTreeNode&& other) noexcept;

    BVHTreeNode(const BVHTreeNode&) = delete;
    BVHTreeNode& operator=(const BVHTreeNode&) = delete;

    bool isLeaf() const;
};

#endif // BVH_UTIL_H
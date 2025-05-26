#ifndef BVH_AGGREGATE_H
#define BVH_AGGREGATE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>  // For std::shared_ptr
#include "utils.h"
#include "bvh_util.h"

class BVHAggregate {
private:
    int max_prims_in_node;
    std::unique_ptr<BVHTreeNode> head;
public:
    BVHAggregate(); // Default constructor declaration
    BVHAggregate(std::vector<std::shared_ptr<hittable>> objs, int max_prims);
    BVHTreeNode* get_head() const;
    std::unique_ptr<BVHTreeNode> sah_recursive(std::vector<BVHPrimitive>& bvhPrimitives);
};

#endif

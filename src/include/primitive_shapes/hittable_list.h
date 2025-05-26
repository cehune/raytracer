#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"
#include "triangle.h"
#include "quadrilateral.h"

#include "../geometry/vec3.h"
#include "../acceleration/bvh_util.h"

using std::make_shared;
using std::shared_ptr;

class hittable_list {
public:
    std::vector<shared_ptr<hittable>> objects;
    mutable int comparisons = 0;

    hittable_list();
    hittable_list(shared_ptr<hittable> object);

    void clear();
    void add(shared_ptr<hittable> object);
    void add(triangleMesh* mesh);
    void add(quadrilateral* quad);

    bool intersect(BVHTreeNode* head, const ray& r, interval ray_t, hit_record& rec) const;
};

#endif // HITTABLE_LIST_H

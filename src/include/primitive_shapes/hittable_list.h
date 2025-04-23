#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H


#include <memory>
#include <vector>
#include "hittable.h"
#include "../acceleration/bvh_util.h"
#include "../geometry/vec3.h"
#include "triangle.h"
#include "quadrilateral.h"

using std::make_shared;
using std::shared_ptr;

class hittable_list{
private:
public:
    std::vector<shared_ptr<hittable>> objects;
    mutable int comparisons = 0;
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    void add(triangleMesh* mesh) {
        for (int i = 0; i < mesh->num_triangles; i++) {
            objects.push_back(std::make_shared<triangle>(mesh, i, mesh->mat));
        }
    }

    void add(quadrilateral *quad) {
        add(&quad->mesh);
    }

bool intersect(BVHTreeNode* head, const ray& r, interval ray_t, hit_record& rec) const {
    hit_record left_rec, right_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;
    comparisons +=1;
    // Check intersection with current node bounds
    if (head->bounds.intersect(r, ray_t)) {
        comparisons +=1;
        if (head->isLeaf()) {
            for (const BVHPrimitive& prim : head->prims) {
                comparisons +=1;
                if (prim.object->intersect(r, interval(ray_t.min, closest_so_far), left_rec)) {
                    hit_anything = true;
                    closest_so_far = left_rec.t;
                    rec = left_rec;
                }
            }
            return hit_anything;
        } else {
            bool left_hit = false;
            if (head->left) {
                left_hit = intersect(head->left.get(), r, ray_t, left_rec);
                if (left_hit) {
                    closest_so_far = left_rec.t;
                    rec = left_rec;
                }
            }

            bool right_hit = false;
            if (head->right) {
                right_hit = intersect(head->right.get(), r, ray_t, right_rec);
                if (right_hit && right_rec.t < closest_so_far) {
                    rec = right_rec;
                    closest_so_far = right_rec.t;
                }
            }
            return left_hit || right_hit;
        }
    }
    return false;
}

};

#endif
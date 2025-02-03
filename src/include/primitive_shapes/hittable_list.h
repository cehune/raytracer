#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H


#include <memory>
#include <vector>
#include "hittable.h"
#include "../acceleration/bvh_util.h"
#include "../geometry/vec3.h"
using std::make_shared;
using std::shared_ptr;

class hittable_list{
private:
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool intersect(BVHTreeNode* head, const ray& r, interval ray_t, hit_record& rec) const {
        hit_record left_rec, right_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        // If there is an intersect with the head bounds (overall structure containing all bounds)
        if (head->bounds.intersect(r, ray_t)) {
            // If it is a leaf, loop through all prims at node
            if (head->isLeaf()) {
                for (const BVHPrimitive& prim : head->prims) {
                    if (prim.object->intersect(r, interval(ray_t.min, closest_so_far), left_rec)) {
                        // Records the closest hit object
                        hit_anything = true;
                        closest_so_far = left_rec.t;
                        rec = left_rec;
                    }
                }
                return hit_anything; 
            } else { // If split node, check for intersection in both child nodes
                bool left_hit = false, right_hit = false;
                if (head->left) {
                    left_hit = intersect(head->left.get(), r, ray_t, left_rec);
                    if (left_hit) {
                        closest_so_far = left_rec.t;
                        rec = left_rec;    
                    }
                }
                if (head->right) {
                    right_hit = intersect(head->right.get(), r, ray_t, right_rec);
                    if (right_hit) {
                        if (right_rec.t < closest_so_far) {
                            rec = right_rec;
                        }
                    }
                }
                // Return hit if either direction
                return left_hit || right_hit;
            }
        }
        return false;
    }
};

#endif
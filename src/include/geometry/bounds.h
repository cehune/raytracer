#ifndef BOUNDS_H
#define BOUNDS_H

#include "vec3.h"
#include "math.h"

struct Bounds3f {
    vec3h pmin, pmax;

    Bounds3f() : pmin(vec3h{0, 0, 0, 1}), pmax(vec3h{0, 0, 0, 1}) {}

    // Constructor to create bounds from min and max points
    Bounds3f(const vec3h& p1, const vec3h& p2){
        pmin = vec_min(p1, p2);
        pmax = vec_max(p1, p2);
    }

    // Example: Expanding the bounding box to include a point
    void expand(const vec3h& p) {
        pmin.x = std::min(pmin.x, p.x);
        pmin.y = std::min(pmin.y, p.y);
        pmin.z = std::min(pmin.z, p.z);
        
        pmax.x = std::max(pmax.x, p.x);
        pmax.y = std::max(pmax.y, p.y);
        pmax.z = std::max(pmax.z, p.z);
    }

    // Example: Checking if a point is inside the bounds
    bool contains(const vec3h& p) const {
        return (p.x >= pmin.x && p.x <= pmax.x &&
                p.y >= pmin.y && p.y <= pmax.y &&
                p.z >= pmin.z && p.z <= pmax.z);
    }

    vec3h diagonal() const { return pmax - pmin; }

    double surface_area() const {
        vec3h d = diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    double volume() const {
        vec3h d = diagonal();
        return d.x * d.y * d.z;
    }

    vec3h lerp_point(vec3h t) const {
        return vec3h(
            lerp(pmin.x, pmax.x, t.x),
            lerp(pmin.y, pmax.y, t.y),
            lerp(pmin.z, pmax.z, t.z), 1);
    }

    int max_dimen() const {
        vec3h d = diagonal();
        if (d.x > d.y && d.x > d.z) return 0;
        else if (d.y > d.z)         return 1;
        else                        return 2;
    }
};

Bounds3f bounds_intersection(const Bounds3f& b1, const Bounds3f& b2) {
    return Bounds3f(vec_max(b1.pmin, b2.pmin), vec_min(b1.pmax, b2.pmax));
}

bool bounds_overlaps(const Bounds3f& b1, const Bounds3f& b2) {
    bool x = (b1.pmax.x >= b2.pmin.x) && (b1.pmin.x <= b2.pmax.x);
    bool y = (b1.pmax.y >= b2.pmin.y) && (b1.pmin.y <= b2.pmax.y);
    bool z = (b1.pmax.z >= b2.pmin.z) && (b1.pmin.z <= b2.pmax.z);
    return (x && y && z);
}


Bounds3f Union(const Bounds3f &b, vec3h& p) {
    Bounds3f ret;
    ret.pmin = vec_min(b.pmin, p);
    ret.pmax = vec_max(b.pmax, p);
    return ret;
}

#endif
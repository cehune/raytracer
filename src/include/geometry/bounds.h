#ifndef BOUNDS_H
#define BOUNDS_H

#include "vec3.h"
#include "math.h"

struct Bounds3f {
    vec3h pmin, pmax;

    Bounds3f() : pmin(vec3h{infinity, infinity, infinity, 1}), pmax(vec3h{-1.0 * infinity, -1.0 * infinity, -1.0 * infinity, 1}) {}
    //Bounds3f() : pmin(vec3h{0.0, 0.0, 0.0, 1}), pmax(vec3h{0.0, 0.0, 0.0, 1}) {}

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

    double inverse_surface_area() const {
        // SAH cost function divides by surface area. might as well just inverse here and multiply
        return 1.0 / (surface_area() > 1e-6f ? surface_area() : 1.0e-6);
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

    double axis_length(int axis) const {
        vec3h p_axis = pmax - pmin;
        if (axis == 0) return p_axis.x;
        if (axis == 1) return p_axis.y;
        if (axis == 2) return p_axis.z;
        throw std::out_of_range("Invalid axis: must be 0, 1, or 2.");
    }

    bool intersect(const ray &r, interval ray_t) const {
        /*
        A modified version of Smit's algorithm by Amy William et al. 
        An Efficient and Robust Ray–Box Intersection Algorithm 
        */

        double t1, t2, tmin, tmax;
        // Check for intersection along the X-axis
        t1 = (pmin.x - r.origin().x) * r.inv_direction().x;
        t2 = (pmax.x - r.origin().x) * r.inv_direction().x;
        if (r.inv_direction().x < 0.0f) std::swap(t1, t2);

        // Initialize tmin and tmax
        tmin = t1;
        tmax = t2;

        // Check for intersection along the Y-axis
        t1 = (pmin.y - r.origin().y) * r.inv_direction().y;
        t2 = (pmax.y - r.origin().y) * r.inv_direction().y;
        if (r.inv_direction().y < 0.0f) std::swap(t1, t2);

        // Update tmin and tmax based on Y-axis intersection
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);

        // If the ray doesn't intersect along the Y-axis, return false
        if (tmin > tmax) return false;

        // Check for intersection along the Z-axis
        t1 = (pmin.z - r.origin().z) * r.inv_direction().z;
        t2 = (pmax.z - r.origin().z) * r.inv_direction().z;
        if (r.inv_direction().z < 0.0f) std::swap(t1, t2);

        // Update tmin and tmax based on Z-axis intersection
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);

        // If the ray doesn't intersect along the Z-axis, return false
        return ((tmin < ray_t.max) && (tmax > ray_t.min));
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

Bounds3f Union(const Bounds3f &b1, const Bounds3f& b2) {
    vec3h newMin = vec_min(b1.pmin, b2.pmin);
    vec3h newMax = vec_max(b1.pmax, b2.pmax);
    return Bounds3f(newMin, newMax);
}


#endif
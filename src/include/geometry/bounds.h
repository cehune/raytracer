#ifndef BOUNDS_H
#define BOUNDS_H

#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include <stdexcept>
#include <algorithm>
#include "math.h"

struct Bounds3f {
    vec3h pmin, pmax;

    // Default constructor: creates an invalid empty box
    Bounds3f();

    // Constructor to create bounds from min and max points
    Bounds3f(const vec3h& p1, const vec3h& p2);

    // Expanding the bounding box to include a point
    void expand(const vec3h& p);

    // Checking if a point is inside the bounds
    bool contains(const vec3h& p) const;

    vec3h diagonal() const;
    double surface_area() const;

    // SAH cost function divides by surface area. might as well just inverse here and multiply
    double inverse_surface_area() const;
    double volume() const;

    vec3h lerp_point(vec3h t) const;

    int max_dimen() const;
    double axis_length(int axis) const;

    // A modified version of Smit's algorithm by Amy Williams et al.
    // An Efficient and Robust Ray–Box Intersection Algorithm
    bool intersect(const ray &r, interval ray_t) const;
};

// Free functions
Bounds3f bounds_intersection(const Bounds3f& b1, const Bounds3f& b2);
bool bounds_overlaps(const Bounds3f& b1, const Bounds3f& b2);
Bounds3f Union(const Bounds3f &b, vec3h& p);
Bounds3f Union(const Bounds3f &b1, const Bounds3f& b2);

#endif // BOUNDS_H

#include "ray.h"
#include <cmath> // for std::fabs

ray::ray()
    : orig(vec3h()), dir(vec3h()), inv_dir(vec3h()), signx(false), signy(false), signz(false) {}

ray::ray(const vec3h& origin, const vec3h& direction)
    : orig(origin), dir(direction) {
    /*
    Set the direction of the ray
    Save the inverse direction for smitts algorithm, intersection with bounding boxes
    Check geometry/bounds for more details or this: https://dl.acm.org/doi/pdf/10.1145/1198555.1198748
    */
    inv_dir = vec3h(
        1.0 / (std::fabs(direction.x) > 1e-6f ? direction.x : 1.0e-6),
        1.0 / (std::fabs(direction.y) > 1e-6f ? direction.y : 1.0e-6),
        1.0 / (std::fabs(direction.z) > 1e-6f ? direction.z : 1.0e-6),
        0);
    signx = (inv_dir.x >= 0);
    signy = (inv_dir.y >= 0);
    signz = (inv_dir.z >= 0);
}

const vec3h& ray::origin() const { return orig; }
const vec3h& ray::direction() const { return dir; }
const vec3h& ray::inv_direction() const { return inv_dir; }
const bool ray::sign_x() const { return signx; }
const bool ray::sign_y() const { return signy; }
const bool ray::sign_z() const { return signz; }

void ray::set_direction(vec3h& d) {
    /*
    Reassign direction, and recalculate inverse direction
    */
    dir = d;
    inv_dir = vec3h(
        1.0 / (std::fabs(d.x) > 1e-6f ? d.x : 1.0e-6),
        1.0 / (std::fabs(d.y) > 1e-6f ? d.y : 1.0e-6),
        1.0 / (std::fabs(d.z) > 1e-6f ? d.z : 1.0e-6),
        0);
    signx = (inv_dir.x >= 0);
    signy = (inv_dir.y >= 0);
    signz = (inv_dir.z >= 0);
}

void ray::set_origin(vec3h& o) {
    orig = o;
}

vec3h ray::line(double t) const {
    return orig + t * dir;
}

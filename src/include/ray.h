#ifndef RAY_H
#define RAY_H

#include "geometry/vec3.h"

class ray {
private:
    vec3h orig;
    vec3h dir;
    vec3h inv_dir;
    bool signx, signy, signz; // https://dl.acm.org/doi/abs/10.1145/1198555.1198748 
public:

    ray() : orig(vec3h()), dir(vec3h()), inv_dir(vec3h()), signx(false), signy(false), signz(false) {}

    ray(const vec3h& origin, const vec3h& direction) : orig(origin), dir(direction) {
          inv_dir = vec3h(
              1.0 / (fabs(direction.x) > 1e-6f ? direction.x : 1.0e-6),
              1.0 / (fabs(direction.y) > 1e-6f ? direction.y : 1.0e-6),
              1.0 / (fabs(direction.z) > 1e-6f ? direction.z : 1.0e-6), 
              0); 
          // Optimized Smitts article uses  1 / 0.0 allowing it to reach infinity. 
          // I don't want unintended behaviour, so using a lower limit of 1e-6f.
          signx = (inv_dir.x >= 0);
          signy = (inv_dir.y >= 0);
          signz = (inv_dir.z >= 0);
    }

    const vec3h& origin() const  { return orig; }
    const vec3h& direction() const { return dir; }
    const vec3h& inv_direction() const { return inv_dir; }
    const bool sign_x() const { return signx; }
    const bool sign_y() const { return signy; }
    const bool sign_z() const { return signy; }

    void set_direction(vec3h& d) {
        dir = d;
        inv_dir = vec3h(
            1.0 / (fabs(d.x) > 1e-6f ? d.x : 1.0e-6),
            1.0 / (fabs(d.y) > 1e-6f ? d.y : 1.0e-6),
            1.0 / (fabs(d.z) > 1e-6f ? d.z : 1.0e-6), 
            0); 
        signx = (inv_dir.x >= 0);
        signy = (inv_dir.y >= 0);
        signz = (inv_dir.z >= 0);
        return;
    }

    void set_origin(vec3h& o) {
        orig = o;
        return;
    }

    vec3h line(double t) const {
        return orig + t*dir;
    }
};

#endif
#ifndef RAY_H
#define RAY_H

#include "geometry/vec3.h"

class ray {
  public:
    vec3h orig;
    vec3h dir;
    vec3h inv_dir;
    bool signx, signy, signz; // https://dl.acm.org/doi/abs/10.1145/1198555.1198748 

    ray(const vec3h& origin, const vec3h& direction) : orig(origin), dir(direction) {
          inv_dir = vec3h(
              1.0f / (fabs(direction.x) > 1e-6f ? direction.x : 1e-6f),
              1.0f / (fabs(direction.y) > 1e-6f ? direction.y : 1e-6f),
              1.0f / (fabs(direction.z) > 1e-6f ? direction.z : 1e-6f), 
              0); 
          // Optimized Smitts article uses  1 / 0.0 allowing it to reach infinity. 
          // I don't want unintended behaviour, so using a lower limit of 1e-6f.
          signx = (inv_dir.x >= 0);
          signy = (inv_dir.y >= 0);
          signz = (inv_dir.z >= 0);
    }

    const vec3h& origin() const  { return orig; }
    const vec3h& direction() const { return dir; }

    vec3h line(double t) const {
        return orig + t*dir;
    }
};

#endif
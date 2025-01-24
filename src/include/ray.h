#ifndef RAY_H
#define RAY_H

#include "geometry/vec3.h"

class ray {
  private:
    vec3h orig;
    vec3h dir;

  public:
    ray() {}

    ray(const vec3h& origin, const vec3h& direction) : orig(origin), dir(direction) {}

    const vec3h& origin() const  { return orig; }
    const vec3h& direction() const { return dir; }

    vec3h line(double t) const {
        return orig + t*dir;
    }
};

#endif
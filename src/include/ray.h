#ifndef RAY_H
#define RAY_H

#include "geometry/vec3.h"

class ray {
  private:
    vec3 orig;
    vec3 dir;

  public:
    ray() {}

    ray(const vec3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    const vec3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }

    vec3 line(double t) const {
        return orig + t*dir;
    }
};

#endif
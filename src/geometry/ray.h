#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
  private:
    point3 origin;
    vec3 dir;

  public:
    ray() {}

    ray(const vec3& origin, const vec3& direction) : origin(origin), dir(direction) {}

    const vec3& getOrigin() const  { return origin; }
    const vec3& getDirection() const { return dir; }

    point3 at(double t) const {
        return origin + t*dir;
    }
};

#endif
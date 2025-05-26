#ifndef RAY_H
#define RAY_H

#include "geometry/vec3.h"

class ray {
private:
    vec3h orig;
    vec3h dir;
    vec3h inv_dir;
    bool signx, signy, signz;

public:
    ray();
    ray(const vec3h& origin, const vec3h& direction);

    const vec3h& origin() const;
    const vec3h& direction() const;
    const vec3h& inv_direction() const;
    const bool sign_x() const;
    const bool sign_y() const;
    const bool sign_z() const;

    void set_direction(vec3h& d);
    void set_origin(vec3h& o);
    vec3h line(double t) const;
};

#endif // RAY_H

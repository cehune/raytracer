#ifndef BXDF_H
#define BXDF_H

#include "../primitive_shapes/hittable.h"
#include "../color.h"

class bxdf {
  public:
    virtual ~bxdf() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }

    virtual color emitted(double u, double v, const vec3h& p) const {
        return color(0,0,0);
    }
};

#endif
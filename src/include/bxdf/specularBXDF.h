/* Directly Reflective Materials */

#ifndef SPECULAR_BXDF_H
#define SPECULAR_BXDF_H

#include "bxdf.h"
#include "../color.h"
#include "../ray.h"
#include "scattering.h"

class specularBXDF : public bxdf {
private:
    color albedo;
    color light_color;
    ray light_direction;

public:
    specularBXDF(const color& albedo);

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

using reflective = specularBXDF;

#endif // SPECULAR_BXDF_H

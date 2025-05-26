/* Refractive Materials defined by snells law */
#ifndef REFRACTIVE_BXDF_H
#define REFRACTIVE_BXDF_H

#include "bxdf.h"
#include "scattering.h"
#include "../color.h"
#include "../ray.h"

class refractiveBXDF : public bxdf {
private:
    color albedo;
    color light_color;
    ray light_direction;
    double eta;

public:
    refractiveBXDF(const color& albedo, double eta);

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

using refractive = refractiveBXDF;

#endif // REFRACTIVE_BXDF_H

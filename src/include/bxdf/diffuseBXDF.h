/* Diffuse Lambertian Materials */

#ifndef DIFFUSE_BXDF_H
#define DIFFUSE_BXDF_H

#include "bxdf.h"
#include "../color.h"
#include "../ray.h"
#include "scattering.h"
#include "../texture.h"

class diffuseBXDF : public bxdf {
private:
    color albedo;
    color light_color;
    ray light_direction;
    shared_ptr<texture> tex;

public:
    diffuseBXDF(const color& albedo);
    diffuseBXDF(shared_ptr<texture> tex);

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

class diffuse_light : public bxdf {
public:
    diffuse_light(shared_ptr<texture> tex);
    diffuse_light(const color& emit);

    color emitted(double u, double v, const vec3h& p) const override;

private:
    shared_ptr<texture> tex;
};

using lambertian = diffuseBXDF;

#endif // DIFFUSE_BXDF_H

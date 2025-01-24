/* Diffuse Lambertian Materials */

#ifndef DIFFUSE_BXDF_H
#define DIFFUSE_BXDF_H

#include "bxdf.h"
#include "../color.h"
#include "../ray.h"
#include "scattering.h"

class diffuseBXDF : public bxdf {
private:
    color albedo;
    color light_color;
    ray light_direction;

public:
    diffuseBXDF(const color& albedo) : albedo(albedo), light_color(color(1,1,1,0)){}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        // TODO: This is not full attenuation formula, scale for light direction too
        attenuation = color(albedo.x * light_color.x, 
            albedo.y * light_color.y, albedo.z * light_color.z, 0);
        vec3h direction = scatter_diffuse(rec.normal);
        scattered = ray(rec.p, direction);
        return true;
    }
};

using lambertian = diffuseBXDF;

#endif
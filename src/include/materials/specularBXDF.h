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
    specularBXDF(const color& albedo) : albedo(albedo), light_color(color(1,1,1)){}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        // TODO: This is not full attenuation formula, scale for light direction too
        attenuation = color(albedo.x * light_color.x, 
            albedo.y * light_color.y, albedo.z * light_color.z);
        vec3 direction = scatter_reflect(r_in.direction(), rec.normal);
        scattered = ray(rec.p, direction);
        return true;
    }
};

using reflective = specularBXDF;

#endif
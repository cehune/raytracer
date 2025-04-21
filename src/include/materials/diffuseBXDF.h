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
    diffuseBXDF(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
    diffuseBXDF(shared_ptr<texture> tex) : tex(tex) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        // TODO: This is not full attenuation formula, scale for light direction too
        attenuation = tex->value(rec.u, rec.v, rec.p);
        vec3h direction = scatter_diffuse(rec.normal);
        scattered = ray(rec.p, direction);
        return true;
    }
};

using lambertian = diffuseBXDF;

#endif
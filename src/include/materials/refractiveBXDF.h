/* Refractive Materials defined by snells law */
#ifndef REFRACTIVE_BXDF_H
#define REFRACTIVE_BXDF_H

#include "bxdf.h"
#include "../color.h"
#include "../ray.h"


class refractiveBXDF : public bxdf {
private:
    color albedo;
    color light_color;
    ray light_direction;
    double eta;

public:
    refractiveBXDF(const color& albedo, double eta) : albedo(albedo), light_color(color(1,1,1)), eta(eta){}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
    // should calculate two rays using snell and fresnel refraction and reflection
    // for now just do refraction lmao TODO add reflection, need to use scatter_reflect func
        attenuation = light_color; // TODO make this awesome and 
        vec3 direction = scatter_refract(r_in.direction(), rec.normal, eta);
        scattered = ray(rec.p, direction);
        return true;
    }

};

using refractive = refractiveBXDF;

#endif
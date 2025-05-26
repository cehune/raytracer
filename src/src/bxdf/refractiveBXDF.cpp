#include "bxdf/refractiveBXDF.h"

refractiveBXDF::refractiveBXDF(const color& albedo, double eta)
    : albedo(albedo), light_color(color(1, 1, 1, 0)), eta(eta) {}

bool refractiveBXDF::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    // should calculate two rays using snell and fresnel refraction and reflection
    // for now just do refraction lmao TODO add reflection, need to use scatter_reflect func
    attenuation = light_color; // TODO make this awesome and 
    vec3h direction = scatter_refract(r_in.direction(), rec.normal, eta);
    scattered = ray(rec.p, direction);
    return true;
}

#include "bxdf/specularBXDF.h"

specularBXDF::specularBXDF(const color& albedo)
    : albedo(albedo), light_color(color(1, 1, 1, 0)) {}

bool specularBXDF::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    // TODO: This is not full attenuation formula, scale for light direction too
    attenuation = color(albedo.x * light_color.x, 
                        albedo.y * light_color.y, 
                        albedo.z * light_color.z, 0);
    vec3h direction = scatter_reflect(r_in.direction(), rec.normal);
    scattered = ray(rec.p, direction);
    return true;
}

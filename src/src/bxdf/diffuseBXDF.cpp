#include "bxdf/diffuseBXDF.h"

/* Diffuse Lambertian Material Implementation */

diffuseBXDF::diffuseBXDF(const color& albedo)
    : tex(make_shared<solid_color>(albedo)) {}

diffuseBXDF::diffuseBXDF(shared_ptr<texture> tex)
    : tex(tex) {}

bool diffuseBXDF::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    // TODO: This is not full attenuation formula, scale for light direction too
    attenuation = tex->value(rec.u, rec.v, rec.p);
    vec3h direction = scatter_diffuse(rec.normal);
    scattered = ray(rec.p, direction);
    return true;
}

/* Diffuse Light Emission */

diffuse_light::diffuse_light(shared_ptr<texture> tex)
    : tex(tex) {}

diffuse_light::diffuse_light(const color& emit)
    : tex(make_shared<solid_color>(emit)) {}

color diffuse_light::emitted(double u, double v, const vec3h& p) const {
    return tex->value(u, v, p);
}

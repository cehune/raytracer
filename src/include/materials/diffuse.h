#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "material.h"
#include "../color.h"
#include "../ray.h"

class diffuse : public material {
private:
    color albedo;
    color light_color;
    ray light_direction;

public:
    diffuse(const color& albedo) : albedo(albedo), light_color(color(1,1,1)){}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        // TODO: This is not full attenuation formula, scale for light direction too
        attenuation = color(albedo.x * light_color.x, 
            albedo.y * light_color.y, albedo.z * light_color.z);
        vec3 direction = rec.normal + random_vec_on_hemisphere(rec.normal);

                // Catch degenerate scatter direction
        if (direction.near_zero())
            direction = rec.normal;
        scattered = ray(rec.p, direction);
        return true;
    }
};

using lambertian = diffuse;

#endif
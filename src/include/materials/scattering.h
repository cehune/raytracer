#ifndef SCATTERING_H
#define SCATTERING_H

#include "bxdf.h"
#include "../color.h"
#include "../ray.h"

vec3 scatter_diffuse(const ray& r_in, vec3 normal) {
    vec3 direction = normal + random_vec_on_hemisphere(normal);
    // Catch degenerate scatter direction
    if (direction.near_zero())
        direction = normal;
    return direction;
}

vec3 scatter_reflect(const ray& r_in, vec3 normal) {
    return -2*dot(r_in.direction(), normal) * normal + r_in.direction();
}

using reflective = specularBXDF;

#endif
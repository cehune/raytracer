#ifndef SCATTERING_H
#define SCATTERING_H

#include <iostream>
#include "bxdf.h"
#include "../color.h"
#include "../ray.h"
#include "../interval.h"
#include "../utils.h"

vec3h scatter_diffuse(vec3h normal) {
    vec3h direction = normal + random_vec_on_hemisphere(normal);
    // Catch degenerate scatter direction
    if (direction.near_zero())
        direction = normal;
    return direction;
}

vec3h scatter_reflect(const vec3h& r_in_dir, vec3h normal) {
    return -2*dot(r_in_dir, normal) * normal + r_in_dir;
}

vec3h scatter_refract(const vec3h& r_in_dir, vec3h normal, double eta) {
    vec3h r_in_norm = r_in_dir.normal_of();
    vec3h norm = normal.normal_of();

    double cos_r_in = clamp(dot(-r_in_norm, norm), -1.0, 1.0); // Correct sign for cos(theta)
    bool entering = cos_r_in > 0.0;

    if (!entering) { 
        norm = -norm; // Flip normal if exiting the medium.
        cos_r_in = -cos_r_in;
    }

    double sin_r_out = sqrt(std::max(0.0, 1.0 - cos_r_in * cos_r_in)) / eta;

    if (sin_r_out >= 1.0) { // Total internal reflection
        return scatter_reflect(r_in_norm, norm); // Reflect function handles reflection vector.
    }

    double cos_r_out = sqrt(std::max(0.0, 1.0 - sin_r_out * sin_r_out));
    return eta * r_in_norm + (eta * cos_r_in - cos_r_out) * norm;
}


double fresnel_dielectric(const vec3h& r_in_dir, vec3h normal, double eta) {
    vec3h r_in_norm = r_in_dir.normal_of();
    vec3h norm = normal.normal_of();

    double cos_r_in = clamp(dot(r_in_norm, norm), -1.0, 1.0); // Clamps to -1.0, 1.0
    double sin_r_in = sqrt(std::max(0.0, 1.0 - cos_r_in * cos_r_in));
    double sin_r_out = sin_r_in / eta;
    
    // Handle total internal reflection
    if (sin_r_out >= 1.0) {
        return 1.0; 
    }
    double cos_r_out = sqrt(std::max(0.0, 1.0 - sin_r_out * sin_r_out)); 

    double r_out_parallel = (eta * cos_r_in - cos_r_out) / (eta * cos_r_in + cos_r_out);
    double r_out_perpendicular = (cos_r_in - eta * cos_r_out) / (cos_r_in + eta * cos_r_out);

    // Return average reflection coefficient
    return (r_out_parallel * r_out_parallel + r_out_perpendicular * r_out_perpendicular) / 2;
}

#endif
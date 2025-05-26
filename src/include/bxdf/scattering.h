#ifndef SCATTERING_H
#define SCATTERING_H

#include <iostream>
#include "../color.h"
#include "../ray.h"
#include "../utils.h"
#include "interval.h"

// Declaration only — moved definitions to scattering.cpp
vec3h scatter_diffuse(vec3h normal);
vec3h scatter_reflect(const vec3h& r_in_dir, vec3h normal);
vec3h scatter_refract(const vec3h& r_in_dir, vec3h normal, double eta);
double fresnel_dielectric(const vec3h& r_in_dir, vec3h normal, double eta);

#endif // SCATTERING_H

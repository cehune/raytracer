#ifndef VEC3H_H
#define VEC3H_H

#include <cmath>
#include "utils.h"
#include <stdexcept>

class vec3h {
public:
    double x, y, z, w;

    vec3h();
    vec3h(double x0, double y0, double z0, double w0);
    vec3h(double x0, double y0, double z0);

    double magnitude() const;
    double operator[](int axis) const;

    vec3h operator-() const;
    vec3h& operator+=(const vec3h& v);
    vec3h& operator*=(double t);
    vec3h& operator/=(double t);

    bool operator==(const vec3h& u);
    bool operator!=(vec3h& u);

    vec3h normal_of() const;
    bool near_zero() const;
    int max_dimen() const;
};

std::ostream& operator<<(std::ostream& out, const vec3h& v);

vec3h operator+(const vec3h& u, const vec3h& v);
vec3h operator-(const vec3h& u, const vec3h& v);
vec3h operator*(const double t, const vec3h& u);
vec3h operator/(const vec3h& u, const double t);

double dot(const vec3h& u, const vec3h& v);
double dot(double ux, double uy, double uz, double vx, double vy, double vz);
double dot(double ux, double uy, double uz, double uw, double vx, double vy, double vz, double vw);

vec3h cross_product(const vec3h& u, const vec3h& v);

vec3h hadamard_product(const vec3h& u, const vec3h& v);

vec3h generate_random_vector();
vec3h generate_random_vector(double min, double max);
vec3h random_unit_vector();
vec3h random_vec_on_hemisphere(const vec3h& normal);
vec3h random_unit_aperture_loc();

vec3h vec_max(const vec3h& u, const vec3h& v);
vec3h vec_min(const vec3h& u, const vec3h& v);

void print(const vec3h& u);

#endif // VEC3H_H

/* 
Vector class for storing points and direction vectors
Is in format of (x,y,z,w) where w is the homogeneous component
if w is 1, then the object is a point. If w is 0, it is a vector direction. 
 */

#ifndef VEC3H_H
#define VEC3H_H

#include <cmath>
#include <iostream>
#include "../utils.h"

class vec3h {
  public:
    double x,y,z,w;

    vec3h() : x(0.0), y(0.0), z(0.0), w(0.0) {};
    vec3h(double x0, double y0, double z0, double w0) : x(x0), y(y0), z(z0), w(w0) {};

    double magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    double operator[](int axis) const {
        if (axis == 0) return x;
        if (axis == 1) return y;
        if (axis == 2) return z;
        throw std::out_of_range("Invalid axis: must be 0, 1, or 2.");
    }
    vec3h operator-() const { return vec3h(-x, -y, -z, w); }
    vec3h& operator+=(const vec3h& v) {
        // Adds a vector onto the current vector
        x += v.x; 
        y += v.y;  
        z += v.z;  
        return *this;
    }

    vec3h& operator*=(double t) {
        // Scales the vector by a constant
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    vec3h& operator/=(double t) {
        if (t != 0) { // Handle division by zero
            return *this *= 1 / t;
        }
        return *this;
    }

    bool operator==(vec3h&u) {
        if (x == u.x && y == u.y && z == u.z) {
            return true;
        }
        return false;
    }
    bool operator!=(vec3h&u) {
        if (x != u.x || y != u.y || z != u.z) {
            return true;
        }
        return false;
    }

    vec3h normal_of() const {
        double mag = magnitude();
        if (mag > 0.0) { return vec3h(x / mag, y / mag, z / mag, w);}
        return vec3h();
    }

    
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        return (std::fabs(x) < 1e-8) && (std::fabs(y) < 1e-8) && (std::fabs(z) < 1e-8);
    }
};

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3h& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline vec3h operator+(const vec3h& u, const vec3h& v) {
    return vec3h(u.x + v.x, u.y + v.y, u.z + v.z, std::max(u.w, v.w));
}

inline vec3h operator-(const vec3h& u, const vec3h& v) {
    return vec3h(u.x - v.x, u.y - v.y, u.z - v.z, std::max(u.w, v.w));
}

inline vec3h operator*(const double t, const vec3h& u) {
    return vec3h(u.x * t, u.y * t, u.z * t, u.w);
}

inline vec3h operator/(const vec3h& u, const double t) {
    return vec3h(u.x / t, u.y / t, u.z / t, u.w);
}

double dot(const vec3h& u, const vec3h& v) {
    // Inner product with two vectors
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

double dot(double ux, double uy, double uz, double vx, double vy, double vz) {
    // Inner product with the xyz values of two vectors
    return ux * vx + uy * vy + uz * vz;
}

double dot(double ux, double uy, double uz, double uw, double vx, double vy, double vz, double vw) {
    // Inner product with the xyz values of two vectors
    return ux * vx + uy * vy + uz * vz + uw * vw;
}

vec3h cross_product(const vec3h& u, const vec3h& v) {
    // Returns cross product
    return vec3h(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x,
        0.0
    );
}

inline vec3h hadamard_product(const vec3h& u, const vec3h& v) {
    return vec3h(u.x * v.x, u.y * v.y, u.z * v.z, std::max(u.w, v.w));
}

inline vec3h generate_random_vector() {
    /* Generate a random vector */
    return vec3h(random_double(), random_double(), random_double(), 0);
}

inline vec3h generate_random_vector(double min, double max) {
        /* 
        Generate a random vector with constraints
        Set min and max to [0,1] for a random unit vector
        */
        return vec3h(random_double(min,max), random_double(min,max), random_double(min,max), 0);
    }

inline vec3h random_unit_vector() {
    while (true) {
        auto p = generate_random_vector(-1,1);
        auto lensq = p.magnitude();
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

inline vec3h random_vec_on_hemisphere(const vec3h& normal) {
    vec3h on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}   

inline vec3h random_unit_aperture_loc() {
    double r = sqrt(random_double(0.0, 1.0)); 
    double theta = random_double(0.0, 2.0 * pi);  
    double x = r * cos(theta);
    double y = r * sin(theta);

    return vec3h(x, y, 0, 1);
}

vec3h vec_max(const vec3h& u, const vec3h& v) {
    /*
    Computes the max vector of two given vectors | if u is (1,1,3) and 
    v is (2,2,1) then the max is (2,2,3)
    */
    return vec3h(std::max(u.x, v.x), std::max(u.y, v.y), std::max(u.z, v.z), 1);
}

vec3h vec_min(const vec3h& u, const vec3h& v) {
    /*
    Computes the max vector of two given vectors | if u is (1,1,3) and 
    v is (2,2,1) then the max is (2,2,3)
    */
    return vec3h(std::min(u.x, v.x), std::min(u.y, v.y), std::min(u.z, v.z), 1);
}

#endif

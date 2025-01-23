#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
  public:
    double x,y,z, magnitude;

    vec3() : x(0), y(0), z(0), magnitude(0) {};
    vec3(double x0, double y0, double z0) : x(x0), y(y0), z(z0), magnitude(std::sqrt(x * x + y * y + z * z)) {};

    double getx() const { return x; }
    double gety() const { return y; }
    double getz() const { return z; }

    vec3 operator-() const { return vec3(-x, -y, -z); }

    vec3& operator+=(const vec3& v) {
        // Adds a vector onto the current vector
        x += v.x; 
        y += v.y;  
        z += v.z;  
        return *this;
    }

    vec3& operator*=(double t) {
        // Scales the vector by a constant
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        if (t != 0) { // Handle division by zero
            return *this *= 1 / t;
        }
        return *this;
    }

    vec3 normal_of() const {
        if (magnitude > 0.0f) { return vec3(x / magnitude, y / magnitude, z / magnitude);}
        return vec3();
    }

    
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const double t, const vec3& u) {
    return vec3(u.x * t, u.y * t, u.z * t);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator/(const vec3& u, const double t) {
    return vec3(u.x / t, u.y / t, u.z / t);
}

double dot(const vec3& u, const vec3& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3 cross_product(const vec3& u, const vec3& v) {
    // Returns cross product
    return vec3(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    );
}


    inline vec3 generate_random_vector() {
            /* Generate a random vector */
            return vec3(random_double(), random_double(), random_double());
        }

    inline vec3 generate_random_vector(double min, double max) {
            /* 
            Generate a random vector with constraints
            Set min and max to [0,1] for a random unit vector
            */
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }

    inline vec3 random_unit_vector() {
        while (true) {
            auto p = generate_random_vector(-1,1);
            auto lensq = p.magnitude;
            if (1e-160 < lensq && lensq <= 1)
                return p / sqrt(lensq);
        }
    }

    inline vec3 random_vec_on_hemisphere(const vec3& normal) {
        vec3 on_unit_sphere = random_unit_vector();
        if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
            return on_unit_sphere;
        else
            return -on_unit_sphere;
    }   

#endif

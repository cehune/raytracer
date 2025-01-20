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

inline vec3 operator/(const vec3& u, const double t) {
    return vec3(u.x / t, u.y / t, u.z / t);
}

double dot(const vec3& u, const vec3& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3 cross(const vec3& u, const vec3& v) {
    // Returns cross product
    return vec3(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    );
}
#endif

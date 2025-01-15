#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
  public:
    float x,y,z;

    vec3() : x(0), y(0), z(0) {};
    vec3(float x0, float y0, float z0) : x(x0), y(y0), z(z0) {};

    float getx() const { return x; }
    float gety() const { return y; }
    float getz() const { return z; }

    vec3 operator-() const { return vec3(-x, -y, -z); }

    vec3& operator+=(const vec3& v) {
        // Adds a vector onto the current vector
        x += v.x; 
        y += v.y;  
        z += v.z;  
        return *this;
    }

    vec3& operator*=(float t) {
        // Scales the vector by a constant
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    vec3& operator/=(float t) {
        if (t != 0) { // Handle division by zero
            return *this *= 1 / t;
        }
        return *this;
    }

    float magnitude() const {
        // Returns the length of the vector
        return std::sqrt(x * x + y * y + z * z);
    }

    float dot(vec3 v) const {
        // Returns dot product
        return x * v.x + y * v.y + z * v.z;
    }

    vec3 cross(const vec3& v) const {
        // Returns cross product
        return vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
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

inline vec3 operator*(const float t, const vec3& u) {
    return vec3(u.x * t, u.y * t, u.z * t);
}

#endif

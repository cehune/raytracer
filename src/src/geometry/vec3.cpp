#include "geometry/vec3.h"

vec3h::vec3h() {};
vec3h::vec3h(double x0, double y0, double z0) : x(x0), y(y0), z(z0), w(0.0) {}
vec3h::vec3h(double x0, double y0, double z0, double w0) : x(x0), y(y0), z(z0), w(w0) {}

double vec3h::magnitude() const {
    return std::sqrt(x*x + y*y + z*z);
}

double vec3h::operator[](int axis) const {
    if (axis == 0) return x;
    if (axis == 1) return y;
    if (axis == 2) return z;
    throw std::out_of_range("Invalid axis index");
}


std::ostream& operator<<(std::ostream& out, const vec3h& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

vec3h operator+(const vec3h& u, const vec3h& v) {
    return vec3h(u.x + v.x, u.y + v.y, u.z + v.z, std::max(u.w, v.w));
}

vec3h operator-(const vec3h& u, const vec3h& v) {
    return vec3h(u.x - v.x, u.y - v.y, u.z - v.z, std::max(u.w, v.w));
}

vec3h operator*(const double t, const vec3h& u) {
    return vec3h(u.x * t, u.y * t, u.z * t, u.w);
}

vec3h operator/(const vec3h& u, const double t) {
    return vec3h(u.x / t, u.y / t, u.z / t, u.w);
}

double dot(const vec3h& u, const vec3h& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

double dot(double ux, double uy, double uz, double vx, double vy, double vz) {
    return ux * vx + uy * vy + uz * vz;
}

double dot(double ux, double uy, double uz, double uw, double vx, double vy, double vz, double vw) {
    return ux * vx + uy * vy + uz * vz + uw * vw;
}

vec3h cross_product(const vec3h& u, const vec3h& v) {
    return vec3h(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x,
        0.0
    );
}

vec3h hadamard_product(const vec3h& u, const vec3h& v) {
    return vec3h(u.x * v.x, u.y * v.y, u.z * v.z, std::max(u.w, v.w));
}

vec3h generate_random_vector() {
    return vec3h(random_double(), random_double(), random_double(), 0);
}

vec3h generate_random_vector(double min, double max) {
    return vec3h(random_double(min, max), random_double(min, max), random_double(min, max), 0);
}

vec3h random_unit_vector() {
    while (true) {
        auto p = generate_random_vector(-1, 1);
        auto lensq = p.magnitude();
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

vec3h random_vec_on_hemisphere(const vec3h& normal) {
    vec3h on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

vec3h random_unit_aperture_loc() {
    double r = sqrt(random_double(0.0, 1.0)); 
    double theta = random_double(0.0, 2.0 * pi);  
    double x = r * cos(theta);
    double y = r * sin(theta);
    return vec3h(x, y, 0, 1);
}
vec3h vec3h::operator-() const {
    return vec3h(-x, -y, -z, w);
}

vec3h& vec3h::operator+=(const vec3h& v) {
    x += v.x; y += v.y; z += v.z;
    return *this;
}

vec3h& vec3h::operator*=(double t) {
    x *= t; y *= t; z *= t;
    return *this;
}

vec3h& vec3h::operator/=(double t) {
    return *this *= 1.0 / t;
}

bool vec3h::operator==(const vec3h& u) {
    return x == u.x && y == u.y && z == u.z && w == u.w;
}

bool vec3h::operator!=(vec3h& u) {
    return !(*this == u);
}

vec3h vec3h::normal_of() const {
    return *this / magnitude();
}

bool vec3h::near_zero() const {
    // Return true if the vector is close to zero in all dimensions.
    return (std::fabs(x) < 1e-8) && (std::fabs(y) < 1e-8) && (std::fabs(z) < 1e-8);
}

int vec3h::max_dimen() const {
    if (x > y && x > z) return 0;
    else if (y > z) return 1;
    else return 2;
}

vec3h vec_max(const vec3h& u, const vec3h& v) {
    return vec3h(std::max(u.x, v.x), std::max(u.y, v.y), std::max(u.z, v.z), 1);
}

vec3h vec_min(const vec3h& u, const vec3h& v) {
    return vec3h(std::min(u.x, v.x), std::min(u.y, v.y), std::min(u.z, v.z), 1);
}

void print(const vec3h& u) {
    std::cout << "x: " << u.x << ", y: " << u.y << ", z: " << u.z << std::endl;
}

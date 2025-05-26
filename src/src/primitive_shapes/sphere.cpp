#include "primitive_shapes/sphere.h"
#include <cmath> // for acos, atan2, sqrt, fmax

sphere::sphere(const vec3h& center, double radius, std::shared_ptr<bxdf> material)
    : center(center), radius(std::fmax(0, radius)), mat(material) {}

sphere::sphere(const vec3h& center, double radius)
    : center(center), radius(std::fmax(0, radius)),
      mat(std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0))) {}

bool sphere::intersect(const ray& r, interval ray_t, hit_record& rec) const {
    vec3h dist = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto h = dot(r.direction(), dist);
    auto c = dot(dist, dist) - radius * radius;
    auto discriminant = h * h - a * c;

    if (discriminant < 0)
        return false;

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.line(rec.t);
    rec.mat = mat;

    vec3h outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);

    return true;
}

void sphere::get_sphere_uv(const vec3h& p, double& u, double& v) {
    // u: azimuthal angle around Y axis from X = -1, ranges [0, 1]
    // v: polar angle from Y = -1 to Y = +1, ranges [0, 1]

    auto theta = std::acos(-p.y);
    auto phi = std::atan2(-p.z, p.x) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}

Bounds3f sphere::bounds() const {
    vec3h rvec(radius, radius, radius, 1);
    return Bounds3f(center - rvec, center + rvec);
}

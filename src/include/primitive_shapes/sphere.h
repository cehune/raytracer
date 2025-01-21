#ifndef SPHERE_H
#define SPHERE_H

#include "./hittable.h"
#include "../vec3.h"

class sphere : public hittable {
private:
    point3 center;
    double radius;

public:
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 dist = center - r.origin();
        auto a = dot(r.direction(), r.direction());
        auto h = dot(r.direction(), dist);
        auto c = dot(dist, dist) - radius*radius;
        auto discriminant = h*h - a*c;

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
        rec.normal = (rec.p - center) / radius;
        // Set the normal, point, and ray direction scalar to hit record
        return true;
    }
};

#endif
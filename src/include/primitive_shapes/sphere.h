#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include "./hittable.h"
#include "../geometry/vec3.h"
#include "../materials/bxdf.h"
#include "../materials/diffuseBXDF.h"

class sphere : public hittable {
private:
    vec3h center;
    double radius;

public:
std::shared_ptr<bxdf> mat; // Material for the sphere

    // Constructor with material
    sphere(const vec3h& center, double radius, std::shared_ptr<bxdf> material) 
        : center(center), radius(std::fmax(0, radius)), mat(material) {}

    // Constructor without explicit material (default material)
    sphere(const vec3h& center, double radius) 
        : center(center), radius(std::fmax(0, radius)), mat(std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0))) {}


    bool intersect(const ray& r, interval ray_t, hit_record& rec) const {
        vec3h dist = center - r.origin();
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
        rec.mat = mat;
        rec.normal = (rec.p - center) / radius;
        // Set the normal, point, and ray direction scalar to hit record
        return true;
    }   
};

#endif
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
        vec3h outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        // Set the normal, point, and ray direction scalar to hit record
        return true;
    }   

    static void get_sphere_uv(const vec3h& p, double& u, double& v) {
        // Normalized angles u: phi, v: theta
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.

        auto theta = std::acos(-p.y);
        auto phi = std::atan2(-p.z, p.x) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }

    Bounds3f bounds() const { 
        vec3h pmin = center - vec3h(radius, radius, radius, 1);  // Center minus radius
        vec3h pmax = center + vec3h(radius, radius, radius, 1);  // Center plus radius
        return Bounds3f(pmin, pmax);  //
    }
};

#endif
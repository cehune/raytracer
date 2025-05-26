#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include "./hittable.h"
#include "../geometry/vec3.h"
#include "../bxdf/bxdf.h"
#include "../bxdf/diffuseBXDF.h"

class sphere : public hittable {
private:
    vec3h center;
    double radius;

public:
    std::shared_ptr<bxdf> mat; // Material for the sphere

    // Constructors
    sphere(const vec3h& center, double radius, std::shared_ptr<bxdf> material);
    sphere(const vec3h& center, double radius); // default material

    bool intersect(const ray& r, interval ray_t, hit_record& rec) const override;
    Bounds3f bounds() const override;

    static void get_sphere_uv(const vec3h& p, double& u, double& v);
};

#endif // SPHERE_H

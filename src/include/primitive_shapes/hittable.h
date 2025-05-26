#ifndef HITTABLE_H
#define HITTABLE_H

#include "../ray.h"
#include "../geometry/bounds.h"
#include <memory>

class bxdf; // Forward declaration for material class

class hit_record {
public:
    vec3h p;                 // Hit point position
    vec3h normal;            // Surface normal at hit point
    std::shared_ptr<bxdf> mat;  // Material at hit point
    double t;                // Ray parameter at hit point
    double u;                // Texture coordinate u
    double v;                // Texture coordinate v
    bool front_face;         // Whether the normal is facing the ray

    /*
        Set up so normals always point outward from the surface. Do this at geometry time
        because we do not have many geometry types right now. Focus on material types.
    */
    void set_face_normal(const ray& r, const vec3h& outward_normal);
};

class hittable {
public:
    virtual ~hittable() = default;

    // Bounding box for acceleration structures
    virtual Bounds3f bounds() const = 0;

    // Ray-geometry intersection
    virtual bool intersect(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif // HITTABLE_H

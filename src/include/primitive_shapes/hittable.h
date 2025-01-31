#ifndef HITTABLE_H
#define HITTABLE_H

#include "../ray.h"
#include "geometry/bounds.h"

class bxdf;

class hit_record {
  public:
    vec3h p; // position
    vec3h normal; // surface normal to the position
    shared_ptr<bxdf> mat;
    double t; // scaling distance
    bool front_face;

    /* Setup so normals always point outward from the surface. Do this at geometry time
    because we do not have much geometry types right now. Focus on material types*/
    void set_face_normal(const ray& r, const vec3h& outward_normal) {
      front_face = dot(r.direction(), outward_normal) < 0; 
      normal = front_face ? outward_normal: -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;
    virtual Bounds3f bounds() const = 0;
    virtual bool intersect(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
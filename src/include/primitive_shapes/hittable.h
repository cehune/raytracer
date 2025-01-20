#ifndef HITTABLE_H
#define HITTABLE_H

#include "../ray.h"

class hit_record {
  public:
    vec3 p; // position
    vec3 normal; // surface normal to the position
    double t; // scaling distance
    bool front_face;

    /* Setup so normals always point outward from the surface. Do this at geometry time
    because we do not have much geometry types right now. Focus on material types*/
    void set_face_normal(const ray& r, const vec3& outward_normal) {
      front_face = dot(r.direction(), outward_normal) < 0; 
      normal = front_face ? outward_normal: -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif
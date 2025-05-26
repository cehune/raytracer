#include "primitive_shapes/hittable.h"

void hit_record::set_face_normal(const ray& r, const vec3h& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

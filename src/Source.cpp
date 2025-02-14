#include <iostream>
#include "include/utils.h"
#include "include/primitive_shapes/hittable.h"
#include "include/primitive_shapes/hittable_list.h"
#include "include/primitive_shapes/sphere.h"
#include "include/primitive_shapes/triangle.h"
#include "include/camera.h"
#include "include/acceleration/bvh_aggregate.h"

int main() {
    std::vector<shared_ptr<hittable>> objects;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0, 0));
    auto material_center = make_shared<lambertian>(color(0, 0.0, 0.0, 0));

    objects.push_back(make_shared<sphere>(vec3h( 0.0, -100.5, -1.0, 1), 100.0, material_ground));
    objects.push_back(make_shared<triangle>(vec3h( -1, 0, -3, 1), vec3h( 1, 0, -3, 1), vec3h( 0, 1, -3, 1), material_center));

    BVHAggregate bvh(objects, 4);
    hittable_list world;

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 300;
    cam.aa_samples_per_px = 5;
    cam.ray_bounces         = 5;

    cam.fov     = 20;
    cam.center = vec3h(13,2,3, 1);
    cam.lookat   = vec3h(0,0,0, 1);
    cam.tilt_angle = 15.0;
    cam.focus_dist    = 10.0;
    cam.render(world, bvh.get_head());
}
#include <iostream>
#include "include/utils.h"
#include "include/primitive_shapes/hittable.h"
#include "include/primitive_shapes/hittable_list.h"
#include "include/primitive_shapes/sphere.h"
#include "include/camera.h"

int main() {
    // World
    hittable_list world;
    world.add(make_shared<sphere>(vec3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100));

    camera cam = camera();
    cam.set_aspect_ratio(16.0 / 9.0);
    cam.set_img_width(400);
    cam.render(world);
}
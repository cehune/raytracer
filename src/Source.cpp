#include <iostream>
#include "include/utils.h"
#include "include/primitive_shapes/hittable.h"
#include "include/primitive_shapes/hittable_list.h"
#include "include/primitive_shapes/sphere.h"
#include "include/camera.h"

int main() {
    // World
    hittable_list world;
    //world.add(make_shared<sphere>(vec3(0,0,-1), 0.5, std::make_shared<diffuse>(color(0.247, 0.416, 0.749))));
    //world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100, std::make_shared<diffuse>(color(0.647, 0.82, 0.639))));

    auto material_ground = make_shared<diffuseBXDF>(color(0.8, 0.8, 0.0, 0));
    auto material_center = make_shared<diffuseBXDF>(color(0.1, 0.2, 0.5, 0));
    auto material_left   = make_shared<specularBXDF>(color(0.8, 0.8, 0.8, 0));
    auto material_right  = make_shared<refractiveBXDF>(color(1,1,1,0), 1.333);

    world.add(make_shared<sphere>(vec3h( 0.5, -100.5, -3.0, 1), 100.0, material_ground));
    world.add(make_shared<sphere>(vec3h( 2.5,    0.0, -3.0, 1),   0.5, material_center));
    //world.add(make_shared<sphere>(point3( 0.5,    0.0, -3.0),   0.5, material_left));
    world.add(make_shared<sphere>(vec3h( 1.0,    0.0, -1.0, 1),   0.5, material_right));
    
    camera cam = camera();
    cam.set_aspect_ratio(16.0 / 9.0);
    cam.set_img_width(500);
    cam.render(world);
}
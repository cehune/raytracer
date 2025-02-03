#include <iostream>
#include "include/utils.h"
#include "include/primitive_shapes/hittable.h"
#include "include/primitive_shapes/hittable_list.h"
#include "include/primitive_shapes/sphere.h"
#include "include/camera.h"
#include "include/acceleration/bvh_aggregate.h"

// Function to compute the height of the tree
int getHeight(BVHTreeNode* node) {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = getHeight(node->left.get());  // Use .get() to access the raw pointer
    int rightHeight = getHeight(node->right.get());

    return std::max(leftHeight, rightHeight) + 1;
}

int main() {
    std::vector<shared_ptr<hittable>> objects;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0, 0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5, 0));
    auto material_left   = make_shared<refractive>(color(0.94, 1, 1, 0), 1.50);
    auto material_bubble = make_shared<refractive>(color(1, 1, 0.98, 0), 1.00 / 1.50);
    auto material_right  = make_shared<reflective>(color(0.8, 0.6, 0.2, 0));

    objects.push_back(make_shared<sphere>(vec3h( 0.0, -100.5, -1.0, 1), 100.0, material_ground));
    objects.push_back(make_shared<sphere>(vec3h( 0.0,    0.0, -1.2, 1),   0.5, material_center));
    objects.push_back(make_shared<sphere>(vec3h(-1.0,    0.0, -1.0, 1),   0.5, material_left));
    objects.push_back(make_shared<sphere>(vec3h(-1.0,    0.0, -1.0, 1),   0.4, material_bubble));
    objects.push_back(make_shared<sphere>(vec3h( 1.0,    0.0, -1.0, 1),   0.5, material_right));

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5, 0));
    objects.push_back(make_shared<sphere>(vec3h(0,-1000,0, 1), 1000, ground_material));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            auto choose_mat = random_double();
            vec3h center(a + 0.9*random_double(), 0.2, b + 0.9*random_double(), 1);

            if ((center - vec3h(4, 0.2, 0, 1)).magnitude() > 0.9) {
                shared_ptr<bxdf> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_unit_vector();
                    sphere_material = make_shared<lambertian>(albedo);
                    objects.push_back(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_unit_vector();
                    sphere_material = make_shared<reflective>(albedo);
                    objects.push_back(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    auto albedo = color(random_double(0.97, 1), random_double(0.97, 1), random_double(0.97, 1), 1);
                    sphere_material = make_shared<refractive>(albedo, 1.5);
                    objects.push_back(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto albedo = random_unit_vector();

    auto material1 = make_shared<refractive>(color(0.98, 0.98, 1, 0), 1.5);
    objects.push_back(make_shared<sphere>(vec3h(0, 1, 0, 1), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1, 0));
    objects.push_back(make_shared<sphere>(vec3h(-4, 1, 0, 1), 1.0, material2));

    auto material3 = make_shared<reflective>(color(0.7, 0.6, 0.5, 0));
    objects.push_back(make_shared<sphere>(vec3h(4, 1, 0, 1), 1.0, material3));

    BVHAggregate bvh(objects, 4);
    hittable_list world;

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 700;
    cam.aa_samples_per_px = 5;
    cam.ray_bounces         = 5;

    cam.fov     = 20;
    cam.center = vec3h(13,2,3, 1);
    cam.lookat   = vec3h(0,0,0, 1);
    cam.tilt_angle = 15.0;
    cam.focus_dist    = 10.0;
    cam.render(world, bvh.get_head());
    //std::cout << objects.size() << std::endl;
    //std::cout << "height" << getHeight(bvh.get_head()) << std::endl;
    std::cout << world.comparisons << " comparisons" << std::endl;
}
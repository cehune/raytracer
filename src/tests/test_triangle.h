#include "../include/geometry/vec3.h"
#include "../include/materials/diffuseBXDF.h"
#include "../include/primitive_shapes/triangle.h"
#include <cassert>

void test_area() {
    vec3h p0(0.0, 0.0, 0.0, 1);
    vec3h p1(1.0, 0.0, 0.0, 1);
    vec3h p2(0.0, 1.0, 0.0, 1);
    triangle tri(p0, p1, p2);
    assert(tri.area(p0, p1, p2) == 0.5);
}

void test_intersection() {
    vec3h p0(0, 0, 0, 1);
    vec3h p1(1, 0, 0, 1);
    vec3h p2(0, 1, 0, 1);
    std::shared_ptr<bxdf> material = std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0));
    triangle tri(p0, p1, p2, material);
    
    ray r(vec3h(0.25, 0.25, -1, 1), vec3h(0, 0, 1, 0));
    interval ray_t(0.001, 100.0);
    hit_record rec;
    assert(tri.intersect(r, ray_t, rec));
}

void test_no_intersection() {
    vec3h p0(0, 0, 0, 1);
    vec3h p1(1, 0, 0, 1);
    vec3h p2(0, 1, 0, 1);
    std::shared_ptr<bxdf> material = std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0));
    triangle tri(p0, p1, p2, material);
    
    ray r(vec3h(2, 2, -1, 1), vec3h(0, 0, 1, 0));
    interval ray_t(0.001, 100.0);
    hit_record rec;
    assert(!tri.intersect(r, ray_t, rec));
}

int run_test_triangle() {
    std::cout << "\n Starting tests for /primative_shapes/triangle\n\n";
    test_area();
    test_intersection();
    test_no_intersection();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

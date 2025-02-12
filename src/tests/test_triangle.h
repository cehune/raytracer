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
    std::cout << "test_area passed!\n";
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
    std::cout << "test_intersection passed!\n";

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
    std::cout << "test_no_intersection passed!\n";

}

void test_dist() {
    vec3h p0(3, 0, -1, 1);
    vec3h p1(3, 1, 0, 1);
    vec3h p2(3, 0, 1, 1);
    // basically a triangle at x = 3, parallel to the yz plane
    std::shared_ptr<bxdf> material = std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0));
    triangle tri(p0, p1, p2, material);
    
    ray r(vec3h(-1, 0.5, 0, 1), vec3h(1, 0, 0, 0));
    interval ray_t(0.001, 100.0);
    hit_record rec;
    assert(tri.intersect(r, ray_t, rec));
    assert(rec.p.x == 3.0 && rec.p.y == 0.5 && rec.p.z == 0.0);
    assert(rec.t == 4.0);
    std::cout << "test_dist passed!\n";

}

#include <iostream>
#include <cassert>

void test_permutation() {
    // Create a triangle and direction vectors
    vec3h p0(3, 0, -1, 1);
    vec3h p1(3, 1, 0, 1);
    vec3h p2(3, 0, 1, 1);
    triangle tri(p0, p1, p2);

    // Test case where direction is along the x-axis
    vec3h direction1(1.0, 0.0, 0.0, 0.0);
    vec3h dirt1, p0t1(1.0, 2.0, 3.0, 1.0), p1t1(4.0, 5.0, 6.0, 1.0), p2t1(7.0, 8.0, 9.0, 1.0);
    tri.permutation(direction1, dirt1, p0t1, p1t1, p2t1);

    // After permutation, x and z should swap
    assert(p0t1.x == 3.0 && p0t1.z == 1.0);
    assert(p1t1.x == 6.0 && p1t1.z == 4.0);
    assert(p2t1.x == 9.0 && p2t1.z == 7.0);
    assert(dirt1.x == 0.0 && dirt1.y == 0.0 && dirt1.z == 1.0);  // Dirt should be (0, 0, 1)

    // Test case where direction is along the y-axis
    vec3h direction2(0.0, 1.0, 0.0, 0.0);
    vec3h dirt2, p0t2(1.0, 2.0, 3.0, 1.0), p1t2(4.0, 5.0, 6.0, 1.0), p2t2(7.0, 8.0, 9.0, 1.0);
    tri.permutation(direction2, dirt2, p0t2, p1t2, p2t2);

    // After permutation, y and z should swap
    assert(p0t2.y == 3.0 && p0t2.z == 2.0);
    assert(p1t2.y == 6.0 && p1t2.z == 5.0);
    assert(p2t2.y == 9.0 && p2t2.z == 8.0);
    assert(dirt2.x == 0.0 && dirt2.y == 0.0 && dirt2.z == 1.0);  // Dirt should be (0, 0, 1)

    // Test case where direction is along the z-axis
    vec3h direction3(0.0, 0.0, 1.0, 1.0);
    vec3h dirt3, p0t3(1.0, 2.0, 3.0, 1.0), p1t3(4.0, 5.0, 6.0, 1.0), p2t3(7.0, 8.0, 9.0, 1.0);
    tri.permutation(direction3, dirt3, p0t3, p1t3, p2t3);

    // No permutation, dirt should be the same as direction
    assert(p0t3.x == 1.0 && p0t3.y == 2.0 && p0t3.z == 3.0);
    assert(p1t3.x == 4.0 && p1t3.y == 5.0 && p1t3.z == 6.0);
    assert(p2t3.x == 7.0 && p2t3.y == 8.0 && p2t3.z == 9.0);
    assert(dirt3.x == 0.0 && dirt3.y == 0.0 && dirt3.z == 1.0);  // Dirt should be (0, 0, 1)

    std::cout << "test_permutation passed!" << std::endl;
}

void test_shear() {
    // Create a triangle and dirt vector
    vec3h p0(3, 0, -1, 1);
    vec3h p1(3, 1, 0, 1);
    vec3h p2(3, 0, 1, 1);
    triangle tri(p0, p1, p2);
    

    // Test case where dirt = (1.0, 2.0, 3.0)
    vec3h dirt1(1.0, 2.0, 3.0, 0);
    vec3h p0t1(1.0, 1.0, 1.0, 1), p1t1(2.0, 2.0, 2.0, 1), p2t1(3.0, 3.0, 3.0, 1);
    tri.shear(dirt1, p0t1, p1t1, p2t1);

    // After shear, x and y coordinates should be adjusted based on the shear factors.
    // We compute expected values manually (based on formula)
    assert(std::fabs(p0t1.x - 2.3333) < 1e-4);
    assert(std::fabs(p0t1.y - 3.3333) < 1e-4);
    assert(std::fabs(p0t1.z - 0.3333) < 1e-4);

    assert(std::fabs(p1t1.x - 4.3333) < 1e-4);
    assert(std::fabs(p1t1.y - 5.3333) < 1e-4);
    assert(std::fabs(p1t1.z - 0.6667) < 1e-4);

    assert(std::fabs(p2t1.x - 6.3333) < 1e-4);
    assert(std::fabs(p2t1.y - 7.3333) < 1e-4);
    assert(std::fabs(p2t1.z - 1.0) < 1e-4);

    std::cout << "test_shear passed!" << std::endl;
}


int run_test_triangle() {
    std::cout << "\n Starting tests for /primative_shapes/triangle\n\n";
    test_area();
    test_intersection();
    test_no_intersection();
    test_dist();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#ifndef TEST_BOUNDS_H
#define TEST_BOUNDS_H

#include <cassert>
#include "../include/geometry/bounds.h"
#include "../include/primitive_shapes/sphere.h"

// Mock vec3h for testing purposes

void test_default_constructor() {
    Bounds3f b;
    assert(b.pmin.x == infinity && b.pmin.y == infinity && b.pmin.z == infinity);
    assert(b.pmax.x == -infinity && b.pmax.y == -infinity && b.pmax.z == -infinity);
    std::cout << "test_default_constructor passed!\n";
}

void test_bounds_from_points() {
    vec3h p1(1, 2, 3, 1);
    vec3h p2(4, 5, 6, 1);
    Bounds3f b(p1, p2);

    assert(b.pmin.x == 1 && b.pmin.y == 2 && b.pmin.z == 3);
    assert(b.pmax.x == 4 && b.pmax.y == 5 && b.pmax.z == 6);
    std::cout << "test_bounds_from_points passed!\n";
}

void test_expand() {
    vec3h p1(1, 1, 1, 1);
    vec3h p2(4, 4, 4, 1);
    Bounds3f b(p1, p2);

    vec3h newPoint(0, 5, 3, 1);
    b.expand(newPoint);

    assert(b.pmin.x == 0 && b.pmin.y == 1 && b.pmin.z == 1);
    assert(b.pmax.x == 4 && b.pmax.y == 5 && b.pmax.z == 4);
    std::cout << "test_expand passed!\n";
}

void test_contains() {
    vec3h p1(1, 1, 1, 1);
    vec3h p2(4, 4, 4, 1);
    Bounds3f b(p1, p2);

    vec3h insidePoint(2, 2, 2, 1);
    vec3h outsidePoint(5, 5, 5, 1);

    assert(b.contains(insidePoint) == true);
    assert(b.contains(outsidePoint) == false);
    std::cout << "test_contains passed!\n";
}

void test_diagonal() {
    vec3h p1(1, 2, 3, 1);
    vec3h p2(4, 6, 8, 1);
    Bounds3f b(p1, p2);

    vec3h diag = b.diagonal();
    assert(diag.x == 3 && diag.y == 4 && diag.z == 5);
    std::cout << "test_diagonal passed!\n";
}

void test_surface_area() {
    vec3h p1(0, 0, 0, 1);
    vec3h p2(2, 3, 4, 1);
    Bounds3f b(p1, p2);

    assert(b.surface_area() == 2 * (2 * 3 + 2 * 4 + 3 * 4));
    std::cout << "test_surface_area passed!\n";
}

void test_volume() {
    vec3h p1(0, 0, 0, 1);
    vec3h p2(2, 3, 4, 1);
    Bounds3f b(p1, p2);

    assert(b.volume() == 2 * 3 * 4);
    std::cout << "test_volume passed!\n";
}

void test_lerp_point() {
    vec3h p1(0, 0, 0, 1);
    vec3h p2(4, 4, 4, 1);
    Bounds3f b(p1, p2);

    vec3h t(0.5, 0.5, 0.5, 1);
    vec3h result = b.lerp_point(t);

    assert(result.x == 2 && result.y == 2 && result.z == 2);
    std::cout << "test_lerp_point passed!\n";
}

void test_max_dimen() {
    vec3h p1(0, 0, 0, 1);
    vec3h p2(1, 5, 3, 1);
    Bounds3f b(p1, p2);

    assert(b.max_dimen() == 1); // y-dimension is largest
    std::cout << "test_max_dimen passed!\n";
}

void test_bounds_intersection() {
    vec3h p1(0, 0, 0, 1), p2(3, 3, 3, 1);
    vec3h q1(1, 1, 1, 1), q2(4, 4, 4, 1);

    Bounds3f b1(p1, p2), b2(q1, q2);
    Bounds3f intersected = bounds_intersection(b1, b2);

    assert(intersected.pmin.x == 1 && intersected.pmin.y == 1 && intersected.pmin.z == 1);
    assert(intersected.pmax.x == 3 && intersected.pmax.y == 3 && intersected.pmax.z == 3);
    std::cout << "test_bounds_intersection passed!\n";
}

void test_bounds_overlaps() {
    vec3h p1(0, 0, 0, 1), p2(3, 3, 3, 1);
    vec3h q1(1, 1, 1, 1), q2(4, 4, 4, 1);
    vec3h r1(4, 4, 4, 1), r2(6, 6, 6, 1);

    Bounds3f b1(p1, p2), b2(q1, q2), b3(r1, r2);

    assert(bounds_overlaps(b1, b2) == true);
    assert(bounds_overlaps(b1, b3) == false);
    std::cout << "test_bounds_overlaps passed!\n";
}

void test_sphere_bounds() {
    sphere s(vec3h(5.0, 0, 0.0, 1), 1.0);
    assert(s.bounds().pmin == vec3h(4.0, -1.0, -1.0, 1));
    std::cout << "test_bounds_overlaps passed!\n";
}

int run_test_bounds() {
    std::cout << "\n Starting tests for /geometry/bounds\n\n";

    test_default_constructor();
    test_bounds_from_points();
    test_expand();
    test_contains();
    test_diagonal();
    test_surface_area();
    test_volume();
    test_lerp_point();
    test_max_dimen();
    test_bounds_intersection();
    test_bounds_overlaps();
    test_sphere_bounds();
    return 0;
}

#endif
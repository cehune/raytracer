#ifndef TEST_TRANFORM_H
#define TEST_TRANFORM_H

#include <cassert>
#include "../include/geometry/transform.h"

bool matricesAreEqual(const squareMatrix<4>& m1, const squareMatrix<4>& m2, double epsilon = 1e-6) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::abs(m1[i][j] - m2[i][j]) > epsilon) {
                return false;
            }
        }
    }
    return true;
}

void test_translation() {
    vec3h delta(1, 2, 3, 1);
    transform result = translate(delta);
    const double expected[4][4] = {
        {1, 0, 0, 1},
        {0, 1, 0, 2},
        {0, 0, 1, 3},
        {0, 0, 0, 1}
    };
    assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
    std::cout << "test_translate passed!\n";
}

void test_scaling() {
    transform result = scale(2, 3, 4);
    const double expected[4][4] = {
        {2, 0, 0, 0},
        {0, 3, 0, 0},
        {0, 0, 4, 0},
        {0, 0, 0, 1}
    };
    assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
    std::cout << "test_scaling passed!\n";
}

void test_rotate_x() {
    double theta = pi / 2; // 90 degrees
    transform result = rotateX(theta);
    const double expected[4][4] = {
        {1, 0, 0, 0},
        {0, 0, -1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1}
    };
    assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
    std::cout << "test_rotate_x passed!\n";
}

void test_rotate_y() {
    double theta = pi / 2; // 90 degrees
    transform result = rotateY(theta);
    const double expected[4][4] = {
        {0, 0, 1, 0},
        {0, 1, 0, 0},
        {-1, 0, 0, 0},
        {0, 0, 0, 1}
    };
    assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
    std::cout << "test_rotate_y passed!\n";
}

void test_rotate_z() {
    double theta = pi / 2; // 90 degrees
    transform result = rotateZ(theta);
    const double expected[4][4] = {
        {0, -1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
    std::cout << "test_rotate_z passed!\n";
}

void test_rotate_axis() {
    vec3h axis(0, 1, 0, 0); // Y-axis
    double theta = pi / 2; // 90 degrees
    transform result = rotateAxis(axis, theta);
    const double expected[4][4] = {
        {0, 0, 1, 0},
        {0, 1, 0, 0},
        {-1, 0, 0, 0},
        {0, 0, 0, 1}
    };
    assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
    std::cout << "test_rotate_axis passed!\n";
}

void test_perspective() {
    double fov = pi / 4; // 45 degrees
    double znear = 1.0;
    double zfar = 10.0;
    transform result = perspective(fov, znear, zfar);
    double tan_half_fov = std::tan(fov / 2.0);
    const double expected[4][4] = {
        {1 / tan_half_fov, 0, 0, 0},
        {0, 1 / tan_half_fov, 0, 0},
        {0, 0, -(zfar + znear) / (zfar - znear), -2 * zfar * znear / (zfar - znear)},
        {0, 0, -1, 0}
    };
    assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
    std::cout << "test_perspective_passed!\n";
}

void test_apply_transform_point() {
    vec3h point(1, 1, 1, 1); // Homogeneous coordinates
    transform t = translate(vec3h(2, 3, 4, 1));
    vec3h transformedPoint = apply_transform(t.m, point);

    // Expected result: translated point
    vec3h expectedPoint(3, 4, 5, 1);

    assert(std::abs(transformedPoint.x - expectedPoint.x) < 1e-6);
    assert(std::abs(transformedPoint.y - expectedPoint.y) < 1e-6);
    assert(std::abs(transformedPoint.z - expectedPoint.z) < 1e-6);
    assert(std::abs(transformedPoint.w - expectedPoint.w) < 1e-6);

    std::cout << "test_apply_transform_point passed!\n";
}

void test_apply_transform_dir() {
    vec3h direction(1, 1, 1, 0); // Homogeneous coordinates with w = 0 (direction vector)
    transform t = translate(vec3h(2, 3, 4, 1));
    vec3h transformedDirection = apply_transform(t.m, direction);

    // Expected result: direction should not be translated (w = 0 remains unchanged)
    vec3h expectedDirection(1, 1, 1, 0);

    assert(std::abs(transformedDirection.x - expectedDirection.x) < 1e-6);
    assert(std::abs(transformedDirection.y - expectedDirection.y) < 1e-6);
    assert(std::abs(transformedDirection.z - expectedDirection.z) < 1e-6);
    assert(std::abs(transformedDirection.w - expectedDirection.w) < 1e-6);

    std::cout << "test_apply_transform_dir passed!\n";
}

int run_test_transform() {
    std::cout << "\n Starting tests for /geometry/transform\n\n";

    test_translation();
    test_scaling();
    test_rotate_x();
    test_rotate_y();
    test_rotate_z();
    test_rotate_axis();
    test_perspective();
    test_apply_transform_point();
    test_apply_transform_dir();

    return 0;
}

#endif

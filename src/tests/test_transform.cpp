#include <cassert>
#include <cmath>
#include <iostream>
#include "../include/utils.h"
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

void runTests() {
    // Test translation
    {
        vec3h delta(1, 2, 3, 1);
        transform result = translate(delta);
        const double expected[4][4] = {
            {1, 0, 0, 1},
            {0, 1, 0, 2},
            {0, 0, 1, 3},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
        std::cout << "Translation test passed.\n";
    }

    // Test scaling
    {
        transform result = scale(2, 3, 4);
        const double expected[4][4] = {
            {2, 0, 0, 0},
            {0, 3, 0, 0},
            {0, 0, 4, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
        std::cout << "Scaling test passed.\n";
    }

    // Test rotateX
    {
        double theta = pi / 2; // 90 degrees
        transform result = rotateX(theta);
        const double expected[4][4] = {
            {1, 0, 0, 0},
            {0, 0, -1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
        std::cout << "RotateX test passed.\n";
    }

    // Test rotateY
    {
        double theta = pi / 2; // 90 degrees
        transform result = rotateY(theta);
        const double expected[4][4] = {
            {0, 0, 1, 0},
            {0, 1, 0, 0},
            {-1, 0, 0, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
        std::cout << "RotateY test passed.\n";
    }

    // Test rotateZ
    {
        double theta = pi / 2; // 90 degrees
        transform result = rotateZ(theta);
        const double expected[4][4] = {
            {0, -1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result.m, squareMatrix<4>(expected)));
        std::cout << "RotateZ test passed.\n";
    }

    // Test rotateAxis
    {
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
        std::cout << "RotateAxis test passed.\n";
    }

    // Test perspective
    {
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
        std::cout << "Perspective test passed.\n";
    }

    // Test apply_transform (for a point)
    {
        vec3h point(1, 1, 1, 1); // Homogeneous coordinates
        transform t = translate(vec3h(2, 3, 4, 1));
        vec3h transformedPoint = apply_transform(t.m, point);

        // Expected result: translated point
        vec3h expectedPoint(3, 4, 5, 1);

        assert(std::abs(transformedPoint.x - expectedPoint.x) < 1e-6);
        assert(std::abs(transformedPoint.y - expectedPoint.y) < 1e-6);
        assert(std::abs(transformedPoint.z - expectedPoint.z) < 1e-6);
        assert(std::abs(transformedPoint.w - expectedPoint.w) < 1e-6);

        std::cout << "Apply transform test passed for point.\n";
    }

    // Test apply_transform (for a direction)
    {
        vec3h direction(1, 1, 1, 0); // Homogeneous coordinates with w = 0 (direction vector)
        transform t = translate(vec3h(2, 3, 4, 1));
        vec3h transformedDirection = apply_transform(t.m, direction);

        // Expected result: direction should not be translated (w = 0 remains unchanged)
        vec3h expectedDirection(1, 1, 1, 0);

        assert(std::abs(transformedDirection.x - expectedDirection.x) < 1e-6);
        assert(std::abs(transformedDirection.y - expectedDirection.y) < 1e-6);
        assert(std::abs(transformedDirection.z - expectedDirection.z) < 1e-6);
        assert(std::abs(transformedDirection.w - expectedDirection.w) < 1e-6);

        std::cout << "Apply transform test passed for direction.\n";
    }
}

int main() {
    runTests();
    return 0;
}
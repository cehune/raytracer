#include <cassert>
#include <cmath>
#include <iostream>
#include "../include/utils.h"
#include "../include/camera.h"
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
        squareMatrix<4> result = translate(delta);
        const double expected[4][4] = {
            {1, 0, 0, 1},
            {0, 1, 0, 2},
            {0, 0, 1, 3},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result, squareMatrix<4>(expected)));
        std::cout << "Translation test passed.\n";
    }

    // Test scaling
    {
        squareMatrix<4> result = scale(2, 3, 4);
        const double expected[4][4] = {
            {2, 0, 0, 0},
            {0, 3, 0, 0},
            {0, 0, 4, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result, squareMatrix<4>(expected)));
        std::cout << "Scaling test passed.\n";
    }

    // Test rotateX
    {
        double theta = pi / 2; // 90 degrees
        squareMatrix<4> result = rotateX(theta);
        const double expected[4][4] = {
            {1, 0, 0, 0},
            {0, 0, -1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result, squareMatrix<4>(expected)));
        std::cout << "RotateX test passed.\n";
    }

    // Test rotateY
    {
        double theta = pi / 2; // 90 degrees
        squareMatrix<4> result = rotateY(theta);
        const double expected[4][4] = {
            {0, 0, 1, 0},
            {0, 1, 0, 0},
            {-1, 0, 0, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result, squareMatrix<4>(expected)));
        std::cout << "RotateY test passed.\n";
    }

    // Test rotateZ
    {
        double theta = pi / 2; // 90 degrees
        squareMatrix<4> result = rotateZ(theta);
        const double expected[4][4] = {
            {0, -1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result, squareMatrix<4>(expected)));
        std::cout << "RotateZ test passed.\n";
    }

    // Test rotateAxis
    {
        vec3h axis(0, 1, 0, 0); // Y-axis
        double theta = pi / 2; // 90 degrees
        squareMatrix<4> result = rotateAxis(axis, theta);
        const double expected[4][4] = {
            {0, 0, 1, 0},
            {0, 1, 0, 0},
            {-1, 0, 0, 0},
            {0, 0, 0, 1}
        };
        assert(matricesAreEqual(result, squareMatrix<4>(expected)));
        std::cout << "RotateAxis test passed.\n";
    }

    // Test perspective
    {
        double fov = pi / 4; // 45 degrees
        double znear = 1.0;
        double zfar = 10.0;
        squareMatrix<4> result = perspective(fov, znear, zfar);
        double tan_half_fov = std::tan(fov / 2.0);
        const double expected[4][4] = {
            {1 / tan_half_fov, 0, 0, 0},
            {0, 1 / tan_half_fov, 0, 0},
            {0, 0, -(zfar + znear) / (zfar - znear), -2 * zfar * znear / (zfar - znear)},
            {0, 0, -1, 0}
        };
        assert(matricesAreEqual(result, squareMatrix<4>(expected)));
        std::cout << "Perspective test passed.\n";
    }
}

int main() {
    runTests();
    return 0;
}
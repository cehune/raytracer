#ifndef TEST_MATH_H
#define TEST_MATH_H

#include <cassert>
#include "../include/geometry/vec3.h"
#include "../include/geometry/math.h"

void test_lerp() {
    double t = 4.0;
    double start = 2.0;
    double end = 3.5;
    double res = 2.0 + 4.0 * 1.5;
    assert(lerp(start, end, t) == res);
    std::cout << "test_lerp passed!\n";

}

int run_test_math() {
    std::cout << "\n Starting tests for /geometry/math\n\n";

    test_lerp();
    return 0;
}

#endif
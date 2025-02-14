
#include "../include/utils.h"
#include <cassert>
#include "../include/geometry/bounds.h"
#include "test_bounds.h"
#include "test_transform.h"
#include "test_math.h"
#include "test_triangle.h"
#include "test_bvh.h"


int main() {
    run_test_bounds();
    run_test_transform();
    run_test_math();
    run_test_bvh();
    //run_test_triangle();
}
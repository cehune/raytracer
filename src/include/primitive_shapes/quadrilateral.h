#ifndef QUAD_H
#define QUAD_H

#include <iostream>
#include "./hittable.h"
#include "triangle.h"
#include "../geometry/vec3.h"
#include "../materials/bxdf.h"
#include "../materials/diffuseBXDF.h"

struct quadrilateral {
    triangleMesh mesh;
    std::shared_ptr<bxdf> mat; // Material for the sphere
    // Constructor with material
    quadrilateral();
    quadrilateral(const vec3h& origin, const vec3h& dir_a, const vec3h& dir_b, std::shared_ptr<bxdf> material)
        : mesh(
            std::vector<vec3h>{ origin, origin + dir_a, origin + dir_b, origin + dir_a + dir_b },
            std::vector<int>{ 0, 1, 2, 1, 2, 3 },
            2, material
          ){}
};

#endif
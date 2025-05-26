#ifndef QUAD_H
#define QUAD_H

#include <iostream>
#include <memory>
#include <vector>

#include "./hittable.h"
#include "triangle.h"
#include "../geometry/vec3.h"
#include "../geometry/transform.h"
#include "../bxdf/bxdf.h"

// Optional: Default material
#include "../bxdf/diffuseBXDF.h"

struct quadrilateral {
    triangleMesh mesh;
    std::shared_ptr<bxdf> mat; // Material for the quad

    // Constructors
    quadrilateral();
    quadrilateral(const vec3h& origin, const vec3h& dir_a, const vec3h& dir_b, std::shared_ptr<bxdf> material);

    void apply_total_transform(transform& t);
};

#endif // QUAD_H

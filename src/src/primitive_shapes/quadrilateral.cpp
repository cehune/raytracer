#include "primitive_shapes/quadrilateral.h"

quadrilateral::quadrilateral(const vec3h& origin, const vec3h& dir_a, const vec3h& dir_b, std::shared_ptr<bxdf> material)
    : mesh(
        std::vector<vec3h>{ origin, origin + dir_a, origin + dir_b, origin + dir_a + dir_b },
        std::vector<int>{ 0, 1, 2, 1, 2, 3 },
        2, material
    ),
    mat(material) {}

void quadrilateral::apply_total_transform(transform& t) {
    mesh.apply_total_transform(t);
}
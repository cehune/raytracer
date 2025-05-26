#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <vector>
#include "./hittable.h"
#include "../geometry/vec3.h"
#include "../bxdf/bxdf.h"
#include "../geometry/bounds.h"
#include "../geometry/transform.h"
#include "../bxdf/diffuseBXDF.h"


struct triangleIntersection {
    double b0, b1, b2, dist;
    bool valid = false;

    triangleIntersection(double a0, double a1, double a2, double t_);
    triangleIntersection();
};

struct triangleMesh {
    std::vector<vec3h> vertices;  // Stores unique vertex positions
    std::vector<int> indices;     // Stores triangle vertex indices (3 per triangle)
    int num_triangles = 0;
    Bounds3f total_bound;
    std::shared_ptr<bxdf> mat; // Shared material for all triangles
    
    triangleMesh();
    // only material given
    triangleMesh(std::shared_ptr<bxdf> mat);
    
    // verts and indices and material given
    triangleMesh(const std::vector<vec3h>& verts, const std::vector<int>& inds, int num_tri,  std::shared_ptr<bxdf> mat);
    
    // material not given, verts and indices given
    triangleMesh(const std::vector<vec3h>& verts, const std::vector<int>& inds, int num_tri);
    
    void apply_total_transform(transform& t); // Apply transform to ALL inner triangles
    Bounds3f bounds();
};


class triangle : public hittable {
private:
    triangleMesh* mesh = nullptr;
    int mesh_index;

    triangleIntersection check_intersection(const ray& r, interval ray_t, vec3h p0, vec3h p1, vec3h p2) const;
    void permutation(vec3h direction, vec3h& dirt, vec3h& p0t, vec3h& p1t, vec3h& p2t) const;
    void shear(vec3h& dirt, vec3h& p0t, vec3h& p1t, vec3h& p2t) const;

public:
    std::shared_ptr<bxdf> mat;

    triangle(triangleMesh* triMesh, int mesh_index, std::shared_ptr<bxdf> material);
    triangle(triangleMesh* triMesh, int mesh_index);
    triangle();

    bool intersect(const ray& r, interval ray_t, hit_record& rec) const override;
    Bounds3f bounds() const override;

    double area(const vec3h& p0, const vec3h& p1, const vec3h& p2) const;
    double area() const;

    friend void test_permutation();
    friend void test_shear();
};

#endif
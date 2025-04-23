#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <vector>
#include "./hittable.h"
#include "../geometry/vec3.h"
#include "../materials/bxdf.h"
#include "../geometry/bounds.h"
#include "../materials/diffuseBXDF.h"

struct triangleIntersection {
    double b0, b1, b2, dist;
    bool valid = false;

    triangleIntersection(double a0, double a1, double a2, double t_) 
        : b0(a0), b1(a1), b2(a2), dist(t_), valid(true) {}

    triangleIntersection() : b0(0), b1(0), b2(0), dist(0), valid(false) {}
};

struct triangleMesh {
    std::vector<vec3h> vertices;  // Stores unique vertex positions
    std::vector<int> indices;     // Stores triangle vertex indices (3 per triangle)
    int num_triangles = 0;
    std::shared_ptr<bxdf> mat; // Shared material for all triangles
    triangleMesh(const std::vector<vec3h>& verts, const std::vector<int>& inds, int num_tri,  std::shared_ptr<bxdf> mat)
        : vertices(verts), indices(inds), num_triangles(num_tri), mat(mat) {}
    triangleMesh(const std::vector<vec3h>& verts, const std::vector<int>& inds, int num_tri)
        : vertices(verts), indices(inds), num_triangles(num_tri), mat(std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0))) {}
    void load_objects(std::vector<shared_ptr<hittable>>& objects);
};

class triangle : public hittable {
private:
    triangleMesh* mesh = nullptr;
    int mesh_index;
    triangleIntersection check_intersection(const ray& r, interval ray_t, vec3h p0, vec3h p1, vec3h p2) const;
    void permutation(vec3h direction, vec3h& dirt, vec3h& p0t, vec3h& p1t, vec3h& p2t) const;
    void shear(vec3h& dirt, vec3h& p0t, vec3h& p1t, vec3h& p2t) const;

    public:
std::shared_ptr<bxdf> mat; // Material for the sphere

    // Constructor with material
    triangle(triangleMesh* triMesh, int mesh_index, std::shared_ptr<bxdf> material) 
        : mesh(triMesh), mesh_index(mesh_index), mat(material) {}

    // Constructor without explicit material (default material)
    triangle(triangleMesh* triMesh, int mesh_index) 
        : mesh(triMesh), mesh_index(mesh_index), mat(std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0))) {}

    triangle() : mesh(nullptr), mesh_index(-1), mat(std::make_shared<diffuseBXDF>(color(0.5, 0.5, 0.5, 0))) {}

    bool intersect(const ray& r, interval ray_t, hit_record& rec) const override;
    Bounds3f bounds() const override;
    double area(const vec3h& p0, const vec3h& p1, const vec3h& p2) const;
    double area() const;
    friend void test_permutation();
    friend void test_shear();
};



bool triangle::intersect(const ray& r, interval ray_t, hit_record& rec) const {
    int i0 = mesh->indices[3 * mesh_index];
    int i1 = mesh->indices[3 * mesh_index + 1];
    int i2 = mesh->indices[3 * mesh_index + 2];
    
    vec3h p0 = mesh->vertices[i0];
    vec3h p1 = mesh->vertices[i1];
    vec3h p2 = mesh->vertices[i2];


    triangleIntersection triIntersection = check_intersection(r, ray_t, p0, p1, p2);
    if (triIntersection.valid == false) return false;

    // Calculate normal
    vec3h normal = cross_product(p1 - p0, p2 - p0).normal_of();
    double t = triIntersection.dist / r.direction().magnitude();

    rec.t = t;
    rec.p = r.line(t);
    rec.normal = normal;
    rec.mat = mat;
    return true;
}

triangleIntersection triangle::check_intersection(const ray& r, interval ray_t, vec3h p0, vec3h p1, vec3h p2) const {
    /* 1. check if degenerate*/
    if (area(p0, p1, p2) == 0) return triangleIntersection();
    
    /* 2. transform triangle and ray coordinate system so that ray direction faces down
    the z axis, and ray origin is treated as (0,0,0). Requires a translation, coordinate permutation
    and shear */
    vec3h dir_t; // _t is for transformed

    // Translation
    vec3h p0_t = p0 - r.origin(); 
    vec3h p1_t = p1 - r.origin();
    vec3h p2_t = p2 - r.origin();

    // Permutation, desire z axis as longest axis
    permutation(r.direction(), dir_t, p0_t, p1_t, p2_t);

    // Shearing
    shear(dir_t, p0_t, p1_t, p2_t);


    // Edge tests
    double e0 = (p1_t.x * p2_t.y) - (p1_t.y * p2_t.x);
    double e1 = (p2_t.x * p0_t.y) - (p2_t.y * p0_t.x);
    double e2 = (p0_t.x * p1_t.y) - (p0_t.y * p1_t.x);
    double det = e0 + e1 + e2;
    if (!((e0 >= 0 && e1 >= 0 && e2 >= 0) || (e0 <= 0 && e1 <= 0 && e2 <= 0))) {
        return triangleIntersection(); // (0,0) not in triangle 
    }

    // Get barycentric coordinates
    double inverse_det = 1.0 / det;
    double b0 = e0 * inverse_det, b1 = e1 * inverse_det, b2 = e2 * inverse_det;
    double dist = b0 * p0_t.z + b1 * p1_t.z + b2 * p2_t.z;

    // Now check if the intersection lies within the valid range for the ray
    if (dist < ray_t.min || dist > ray_t.max) {
        return triangleIntersection(); // Intersection is outside the valid ray range
    }

    // Return the barycentric coordinates (b0, b1, b2) and the hit distance t
    return triangleIntersection{b0, b1, b2, dist};  // This object would hold the intersection information
}

void triangle::permutation(vec3h direction, vec3h& dirt, vec3h& p0t, vec3h& p1t, vec3h& p2t) const {
    int longest_axis = direction.max_dimen();
    double temp = 0.0;
    if (longest_axis == 0) {
        dirt.x = direction.z;
        dirt.y = direction.y;
        dirt.z = direction.x;

        temp = p0t.x; // permute p0t
        p0t.x = p0t.z;
        p0t.z = temp;

        temp = p1t.x; //permute p1t
        p1t.x = p1t.z;
        p1t.z = temp;

        temp = p2t.x; //permute p2t
        p2t.x = p2t.z;
        p2t.z = temp;

    } else if (longest_axis == 1) {
        dirt.x = direction.x;
        dirt.y = direction.z;
        dirt.z = direction.y;

        temp = p0t.y; // permute p0t
        p0t.y = p0t.z;
        p0t.z = temp;

        temp = p1t.y; // permute p1t
        p1t.y = p1t.z;
        p1t.z = temp;

        temp = p2t.y; // permute p2t
        p2t.y = p2t.z;
        p2t.z = temp;
    } else {
        dirt.x = direction.x;
        dirt.y = direction.y;
        dirt.z = direction.z;
    }
}

void triangle::shear(vec3h& dirt, vec3h& p0t, vec3h& p1t, vec3h& p2t) const{
    double shear_x = -dirt.x / dirt.z;
    double shear_y = -dirt.y / dirt.z;
    double shear_z = 1.0 / dirt.z;

    p0t.x += p0t.z * shear_x;
    p0t.y += p0t.z * shear_y;
    p0t.z *= shear_z;
    p1t.x += p1t.z * shear_x;
    p1t.y += p1t.z * shear_y;
    p1t.z *= shear_z;
    p2t.x += p2t.z * shear_x;
    p2t.y += p2t.z * shear_y;
    p2t.z *= shear_z;
}

double triangle::area(const vec3h& p0, const vec3h& p1, const vec3h& p2) const {    
    // Calculates the area of a triangle given the 3 points that define it's vertice
    // recall cross length gives area of parallelogram, so we half that
    vec3h a = p1 - p0;
    vec3h b = p2 - p0;

    return 0.5 * cross_product(a,b).magnitude();
}


double triangle::area() const {    
    // Calculates the area of a triangle given the 3 points that define it's vertice
    // recall cross length gives area of parallelogram, so we half that
    int i0 = mesh->indices[3 * mesh_index];
    int i1 = mesh->indices[3 * mesh_index + 1];
    int i2 = mesh->indices[3 * mesh_index + 2];
    
    vec3h p0 = mesh->vertices[i0];
    vec3h p1 = mesh->vertices[i1];
    vec3h p2 = mesh->vertices[i2];
    
    vec3h a = p1 - p0;
    vec3h b = p2 - p0;

    return 0.5 * cross_product(a,b).magnitude();
}

Bounds3f triangle::bounds() const  { 
    int i0 = mesh->indices[3 * mesh_index];
    int i1 = mesh->indices[3 * mesh_index + 1];
    int i2 = mesh->indices[3 * mesh_index + 2];
    
    vec3h p0 = mesh->vertices[i0];
    vec3h p1 = mesh->vertices[i1];
    vec3h p2 = mesh->vertices[i2];

    Bounds3f b = Bounds3f(p0, p1);
    b.expand(p2);
    return b;
}

void triangleMesh::load_objects(std::vector<shared_ptr<hittable>>& objects) {
    for (int i = 0; i < num_triangles; i++) {
        objects.push_back(make_shared<triangle>(this, i, mat));
    }
}

#endif
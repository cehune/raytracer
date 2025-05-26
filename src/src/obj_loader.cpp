#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader.h"
#include "lib/tiny_obj_loader.h"
#include "geometry/vec3.h"
#include <iostream>
#include <vector>

int obj_loader::load_into_triangleMesh(const std::string& filename, triangleMesh& mesh) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<vec3h> vertices;
    std::vector<int> indices;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "ERR: " << err << std::endl;
    }
    if (!ret) {
        std::cerr << "Failed to load OBJ file.\n";
        return -1;
    }

    // 1. Center and load vertices. 
    vertices.clear();
    int num_vertices = (int)attrib.vertices.size() / 3;

    double x_sum = 0, y_sum = 0, z_sum = 0;
    for (int i = 0; i < num_vertices; i++) {
        x_sum += attrib.vertices[3*i + 0];
        y_sum += attrib.vertices[3*i + 1];
        z_sum += attrib.vertices[3*i + 2];
    }
    // Reallign meshes so that the geometric center is at 0,0,0
    double x_center = x_sum / num_vertices;
    double y_center = y_sum / num_vertices;
    double z_center = z_sum / num_vertices;

    for (int i = 0; i < num_vertices; i++) {
        vec3h curr(
            attrib.vertices[3*i + 0] - x_center,
            attrib.vertices[3*i + 1] - y_center,
            attrib.vertices[3*i + 2] - z_center,
            1.0);
        vertices.push_back(curr);
    }

    // 2. Load triangle/quad indices
    indices.clear();
    for (auto& shape : shapes) {
        int index_offset = 0;

        for (int i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
            int fv = shape.mesh.num_face_vertices[i];

            if (fv == 3) {
                // Handle when faces given as triangles
                for (int j = 0; j < 3; j++) {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + j];
                    indices.push_back(idx.vertex_index);
                }
            } else if (fv == 4) {
                // Handle cases where quads are givne
                int idx0 = shape.mesh.indices[index_offset + 0].vertex_index;
                int idx1 = shape.mesh.indices[index_offset + 1].vertex_index;
                int idx2 = shape.mesh.indices[index_offset + 2].vertex_index;
                int idx3 = shape.mesh.indices[index_offset + 3].vertex_index;

                indices.push_back(idx0);
                indices.push_back(idx1);
                indices.push_back(idx2);
                indices.push_back(idx0);
                indices.push_back(idx2);
                indices.push_back(idx3);
            }

            index_offset += fv;
        }
    }

    mesh.vertices = vertices;
    mesh.indices = indices;
    mesh.num_triangles = static_cast<int>(indices.size()) / 3;

    return mesh.num_triangles;
}

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "lib/tiny_obj_loader.h"
#include <iostream>
#include "primitive_shapes/triangle.h"
#include "geometry/vec3.h"

class obj_loader {
    public:
    int load_into_triangleMesh(const std::string& filename, triangleMesh& mesh) {
        
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

        // 1. Load all unique vertices
        // obj file format is v x_val y_val z_val w (optional, idc about w)
        vertices.clear();
        int num_vertices = (int)attrib.vertices.size() / 3;

        double x_sum = 0, y_sum = 0, z_sum = 0;
        for (int i = 0; i < num_vertices; i++) {
            x_sum += attrib.vertices[3*i + 0];
            y_sum += attrib.vertices[3*i + 1];
            z_sum += attrib.vertices[3*i + 2];
        }
        double x_center = x_sum / num_vertices;
        double y_center = y_sum / num_vertices;
        double z_center = z_sum / num_vertices;

        // Shift all vertices
        vertices.clear();
        for (int i = 0; i < num_vertices; i++) {
            vec3h curr = vec3h(
                attrib.vertices[3*i + 0] - x_center,
                attrib.vertices[3*i + 1] - y_center, 
                attrib.vertices[3*i + 2] - z_center, 
                1.0);
            vertices.push_back(curr); 
        }

        // 2. Load indices
        indices.clear();
        for (auto& shape : shapes) {
            int index_offset = 0;

            for (int i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
                int fv = shape.mesh.num_face_vertices[i];  // face vertex count

                if (fv == 3) {
                    // Triangle
                    for (int j = 0; j < 3; j++) {
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + j];
                        indices.push_back(idx.vertex_index);
                    }
                }
                else if (fv == 4) {
                    // Quad → split into two triangles: (0, 1, 2) and (0, 2, 3)
                    int idx0 = shape.mesh.indices[index_offset + 0].vertex_index;
                    int idx1 = shape.mesh.indices[index_offset + 1].vertex_index;
                    int idx2 = shape.mesh.indices[index_offset + 2].vertex_index;
                    int idx3 = shape.mesh.indices[index_offset + 3].vertex_index;
                
                    // First triangle
                    indices.push_back(idx0);
                    indices.push_back(idx1);
                    indices.push_back(idx2);
                
                    // Second triangle
                    indices.push_back(idx0);
                    indices.push_back(idx2);
                    indices.push_back(idx3);
                }

                index_offset += fv;
            }
        }
        mesh.vertices = vertices;
        mesh.indices = indices;
        mesh.num_triangles = (int) indices.size() / 3;
        return (int) indices.size() / 3; // this is num of triangles
    }
};

#endif
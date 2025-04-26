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
        for (int i = 0; i < num_vertices; i++) {
            vec3h curr = vec3h(
                attrib.vertices[3*i + 0],
                attrib.vertices[3*i + 1], 
                attrib.vertices[3*i + 2], 
                1.0);
            if (i == 0) {
                //print(curr);
                //print(curr.normal_of());
            }
            vertices.push_back(curr.normal_of()); 
        }

        // 2. Load indices
        indices.clear();
        for (auto& shape : shapes) {
            int index_offset = 0;

            for (int i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
                int fv = shape.mesh.num_face_vertices[i];  // face vertex count

                for (int j = 0; j < fv; j++) {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + j];
                    indices.push_back(idx.vertex_index);  // vertex_index points into attrib.vertices
                }

                index_offset += fv;
            }
        }
        mesh.vertices = vertices;
        //print(mesh.vertices[0]);
        mesh.indices = indices;
        mesh.num_triangles = (int) indices.size() / 3;
        return (int) indices.size() / 3; // this is num of triangles
    }
};

#endif
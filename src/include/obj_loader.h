#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include "primitive_shapes/triangle.h"

class obj_loader {
public:
    int load_into_triangleMesh(const std::string& filename, triangleMesh& mesh);
};

#endif // OBJ_LOADER_H
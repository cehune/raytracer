cmake_minimum_required(VERSION 3.10)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)

# Shared source files
set(SHARED_SRCS
    src/src/camera.cpp
    src/src/color.cpp
    src/src/interval.cpp
    src/src/obj_loader.cpp
    src/src/ray.cpp
    src/src/texture.cpp
    src/src/rtw_stb_image.cpp

    src/src/primitive_shapes/hittable_list.cpp
    src/src/primitive_shapes/hittable.cpp
    src/src/primitive_shapes/quadrilateral.cpp
    src/src/primitive_shapes/sphere.cpp    
    src/src/primitive_shapes/triangle.cpp

    src/src/geometry/bounds.cpp
    src/src/geometry/math.cpp
    src/src/geometry/matrix.cpp
    src/src/geometry/transform.cpp    
    src/src/geometry/vec3.cpp

    src/src/acceleration/bvh_util.cpp
    src/src/acceleration/bvh_aggregate.cpp

    src/src/bxdf/scattering.cpp
    src/src/bxdf/diffuseBXDF.cpp
    src/src/bxdf/refractiveBXDF.cpp
    src/src/bxdf/specularBXDF.cpp
)

add_executable(raytracer src/Source.cpp ${SHARED_SRCS})


# Test executable
add_executable(tests src/tests/run_tests.cpp ${SHARED_SRCS})


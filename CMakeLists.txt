cmake_minimum_required(VERSION 3.10)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)

add_executable(raytracer src/Source.cpp)

add_executable(tests src/tests/test_transform.cpp)

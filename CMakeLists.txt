cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(mesh2depth VERSION 0.1.0 LANGUAGES CXX C)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 17)
add_executable(mesh2depth main.cpp deps/glad/src/glad.c Model.cpp stb_image_write.c)

target_link_libraries(mesh2depth PUBLIC
    glfw
    glm::glm)
target_include_directories(mesh2depth PUBLIC
    glfw
    glm::glm
    deps/glad/include)

add_library(libmesh2depth deps/glad/src/glad.c Model.cpp stb_image_write.c)
target_link_libraries(libmesh2depth PUBLIC
    glfw
    glm::glm)
target_include_directories(libmesh2depth PUBLIC
    glfw
    glm::glm
    deps/glad/include)

add_executable(m2d_test offline_test.cpp)
target_link_libraries(m2d_test PUBLIC
    libmesh2depth)

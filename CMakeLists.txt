cmake_minimum_required(VERSION 3.20)

project(volmesh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

enable_testing()
# find_package(GTest CONFIG REQUIRED)
# message(FATAL_ERROR "GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")
set(VOLMESH_LIB_NAME "volmesh")

add_library(${VOLMESH_LIB_NAME}
            STATIC
            src/volmesh/aabb.cpp
            src/volmesh/halfedge.cpp
            src/volmesh/index.cpp
            src/volmesh/logger.cpp
            src/volmesh/mathutils.cpp
            src/volmesh/mergelist.cpp
            src/volmesh/sampletetmeshes.cpp
            src/volmesh/signeddistancefield.cpp
            src/volmesh/stlserializer.cpp
            src/volmesh/tetmesh.cpp
            src/volmesh/tetrahedra.cpp
            src/volmesh/trianglemesh.cpp
            src/volmesh/voxel.cpp)

target_include_directories(${VOLMESH_LIB_NAME} PRIVATE
                           ${CMAKE_SOURCE_DIR}/include)

# target_compile_options(${VOLMESH_LIB_NAME} PRIVATE -g)
target_link_libraries(${VOLMESH_LIB_NAME}
                      PRIVATE
                      Eigen3::Eigen
                      fmt::fmt)

add_subdirectory(test)
add_subdirectory(apps)
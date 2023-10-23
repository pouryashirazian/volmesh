cmake_minimum_required(VERSION 3.20)

project(volmesh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(cxxopts REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

enable_testing()
# find_package(GTest CONFIG REQUIRED)
# message(FATAL_ERROR "GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")
set(LIB_NAME "volmesh")

add_library(${LIB_NAME}
            STATIC
            src/volmesh/halfedge.cpp
            src/volmesh/index.cpp
            src/volmesh/mathutils.cpp
            src/volmesh/sampletetmeshes.cpp
            #  src/volmesh/stlserializer.cpp
            src/volmesh/tetmesh.cpp
            src/volmesh/tetrahedra.cpp
            src/volmesh/voxel.cpp)

target_include_directories(${LIB_NAME} PRIVATE
                           ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${LIB_NAME}
                      PRIVATE
                      Eigen3::Eigen
                      fmt::fmt)

add_subdirectory(test)
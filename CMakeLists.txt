#------------------------------------------------------------------------------
# Copyright (c) Pourya Shirazian
# All rights reserved.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(volmesh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#----------------------------------------------------------------------------------
# All build options
#----------------------------------------------------------------------------------
# We disabled building docs by default as it requires doxygen, sphinx and dot which
# may not be available on the user system.
option(BUILD_DOCS "Build documentation" OFF)
mark_as_advanced(BUILD_DOCS)

#----------------------------------------------------------------------------------
# Finds external packages
#----------------------------------------------------------------------------------
find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
enable_testing()

#----------------------------------------------------------------------------------
# Builds the main library
#----------------------------------------------------------------------------------
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

target_link_libraries(${VOLMESH_LIB_NAME}
                      PRIVATE
                      Eigen3::Eigen
                      fmt::fmt)

#----------------------------------------------------------------------------------
# Builds tests
#----------------------------------------------------------------------------------
add_subdirectory(test)

#----------------------------------------------------------------------------------
# Builds apps
#----------------------------------------------------------------------------------
add_subdirectory(apps)

#----------------------------------------------------------------------------------
# Builds docs. Only enable if you have already installed doxygen, sphinx, and dot.
#----------------------------------------------------------------------------------
if(BUILD_DOCS)
  set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  add_subdirectory(docs)
endif()
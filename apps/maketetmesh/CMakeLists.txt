#------------------------------------------------------------------------------
# Copyright (c) Pourya Shirazian
# All rights reserved.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#------------------------------------------------------------------------------

project(maketetmesh)
add_executable(${PROJECT_NAME} maketetmesh.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} ${VOLMESH_LIB_NAME}
                      Eigen3::Eigen
                      fmt::fmt)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

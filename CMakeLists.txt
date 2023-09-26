cmake_minimum_required(VERSION 3.20)

project(volmesh LANGUAGES CXX)

find_package(cxxopts REQUIRED)
find_package(Eigen3 REQUIRED)

enable_testing()
find_package(GTest CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME}
            STATIC
            src/volmesh/halfedge.cpp
            src/volmesh/keys.cpp
            #  src/volmesh/stlserializer.cpp
            src/volmesh/tetmesh.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
                      PRIVATE
                      cxxopts::cxxopts
                      Eigen3::Eigen)


add_subdirectory(test)
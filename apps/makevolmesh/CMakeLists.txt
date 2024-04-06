project(makevolmesh)
add_executable(${PROJECT_NAME} makevolmesh.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} ${VOLMESH_LIB_NAME}
                      Eigen3::Eigen
                      fmt::fmt)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

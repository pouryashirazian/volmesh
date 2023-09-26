add_executable(test_tetmesh test_tetmesh.cpp)

target_include_directories(test_tetmesh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(test_tetmesh PRIVATE GTest::gtest GTest::gtest_main)
add_test(AllTestsInMain test_tetmesh)
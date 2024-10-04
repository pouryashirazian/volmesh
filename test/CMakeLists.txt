#------------------------------------------------------------------------------
# Copyright (c) Pourya Shirazian
# All rights reserved.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#------------------------------------------------------------------------------

# Define test cases and register each test to test suites
file (GLOB TEST_SUITE "test_*.cpp")

find_package(GTest REQUIRED)

foreach (TEST ${TEST_SUITE})
  # Extract file name
  get_filename_component (TEST_FILE_NAME ${TEST} NAME)
  get_filename_component (aTEST ${TEST} NAME_WE)

  # Define test unit
  add_executable (${aTEST} ${TEST_FILE_NAME})

  # Define dependencies
  target_include_directories(${aTEST} PRIVATE
                             ${CMAKE_SOURCE_DIR}/include/)

  target_link_libraries(${aTEST} PRIVATE GTest::gtest_main volmesh)
  add_test(${aTEST} ${aTEST})

endforeach()
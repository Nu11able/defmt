# FetchContent requires cmake >=3.11
# see https://cmake.org/cmake/help/v3.11/module/FetchContent.html
cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

include(FetchContent)

option(SYSTEM_GOOGLE_TEST "Use system copy of Google Test" OFF)

if (NOT SYSTEM_GOOGLE_TEST)
  FetchContent_Declare(
    GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
  )
  FetchContent_MakeAvailable(GTest)
else ()
  find_package(GTest REQUIRED)
endif()

enable_testing()

add_executable(test_defmt
  test.cpp
)

target_link_libraries(test_defmt PRIVATE GTest::gtest GTest::gtest_main defmt)

add_test(NAME test_defmt COMMAND test_defmt)
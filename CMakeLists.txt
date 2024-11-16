cmake_minimum_required(VERSION 3.8...3.26)
project(DEFMT CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(DEFMT_TEST "Enable tests" ON)
if(DEFMT_TEST)
  enable_testing()
  add_subdirectory(tests)
else(DEFMT_TEST)
  message(STATUS "Tests are disabled. Set DEFMT_TEST to ON to run tests.")
endif(DEFMT_TEST)

# add_definitions(-DDEFMT_DEBUG)

add_library(defmt INTERFACE)
add_library(defmt::defmt ALIAS defmt)
target_include_directories(
  defmt
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_executable(example example/example.cpp)
target_link_libraries(example defmt)
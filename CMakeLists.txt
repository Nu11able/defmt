cmake_minimum_required(VERSION 3.8...3.26)

# Fallback for using newer policies on CMake <3.12.
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

# Determine if fmt is built as a subproject (using add_subdirectory)
# or if it is the master project.
if (NOT DEFINED FMT_MASTER_PROJECT)
  set(FMT_MASTER_PROJECT OFF)
  if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(FMT_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
  endif ()
endif ()


project(DEFMT CXX)
set(CMAKE_CXX_STANDARD 17)

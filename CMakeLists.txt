cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(mteval)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON)

# Boost configurations
if(DEFINED BOOST_ROOT OR DEFINED BOOSTROOT OR DEFINED ENV{BOOST_ROOT} OR DEFINED ENV{BOOSTROOT})
  set(Boost_NO_BOOST_CMAKE ON)
  set(Boost_NO_SYSTEM_PATHS ON)
  set(Boost_REALPATH ON)
endif()
find_package(Boost 1.48.0 REQUIRED COMPONENTS
  program_options
  unit_test_framework)
MESSAGE("-- Boost include directory: " ${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

enable_testing()

add_subdirectory(bin)
add_subdirectory(mteval)
add_subdirectory(test)

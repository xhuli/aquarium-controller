cmake_minimum_required(VERSION 3.13)
project(AquariumController)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CMakeInclude.txt)
#include(CMakeListsPrivate.txt)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

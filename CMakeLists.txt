cmake_minimum_required(VERSION 3.13)
project(AController)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(ArduinoLibraries SHARED IMPORTED)

include_directories(include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

add_executable(ArduinoAtoStation examples/Arduino/AtoStation/main.cpp)

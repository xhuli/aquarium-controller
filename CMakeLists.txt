cmake_minimum_required(VERSION 3.13)
project(AController)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

SET(ARDUINO_DIR "c:\\Program Files (x86)\\Arduino")
#SET(CMAKE_C_COMPILER "${ARDUINO_DIR}\\hardware\\tools\\avr\\bin\\avr-gcc.exe")
#SET(CMAKE_CXX_COMPILER "${ARDUINO_DIR}\\hardware\\tools\\avr\\bin\\avr-g++.exe")

add_library(ArduinoLibraries SHARED IMPORTED)
#include_directories("${ARDUINO_DIR}\\hardware\\arduino\\avr\\cores\\arduino")

include_directories(include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

add_executable(ArduinoAtoStation examples/Arduino/AtoStation/main.cpp)

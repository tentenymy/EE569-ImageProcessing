cmake_minimum_required(VERSION 3.3)
project(p3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES p3_code/p3_main.cpp p3_code/Image.cpp p3_code/Image.h p3_code/Morphology.cpp p3_code/Morphology.h)
add_executable(p3 ${SOURCE_FILES})
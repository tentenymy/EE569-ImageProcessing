cmake_minimum_required(VERSION 3.3)
project(p2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES p2_code/p2_main.cpp p2_code/Image.h p2_code/Image.cpp p2_code/Halftoning.cpp p2_code/Halftoning.h)
add_executable(p2 ${SOURCE_FILES})
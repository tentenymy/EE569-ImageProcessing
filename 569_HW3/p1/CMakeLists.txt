cmake_minimum_required(VERSION 3.3)
project(p1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES p1_code/main.cpp p1_code/Image.cpp p1_code/Image.h)
add_executable(p1 ${SOURCE_FILES})
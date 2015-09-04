cmake_minimum_required(VERSION 3.3)
project(HW1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp helper.cpp helper.h)
add_executable(HW1 ${SOURCE_FILES})
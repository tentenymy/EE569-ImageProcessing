cmake_minimum_required(VERSION 3.3)
project(569_HW1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    hw1_helper.cpp
    hw1_helper.h
    p1_bilinear.cpp
    p1_MHC.cpp
    p1_resize.cpp
    p2_cumulative.cpp
    p2_oilpainting.cpp
    p2_transfer.cpp
    p3_gaussian.cpp
    p3_guided.cpp
    p3_median.cpp)

add_executable(569_HW1 ${SOURCE_FILES})
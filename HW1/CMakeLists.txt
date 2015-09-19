cmake_minimum_required(VERSION 3.3)
project(HW1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES p1_resize.cpp hw1_helper.cpp hw1_helper.h p1_bilinear.cpp p1_MHC.cpp example.cpp p2_transfer.cpp p2_enhance.h p2_cumulative.cpp cumulative.h)
add_executable(HW1 ${SOURCE_FILES} p1_bilinear.cpp)
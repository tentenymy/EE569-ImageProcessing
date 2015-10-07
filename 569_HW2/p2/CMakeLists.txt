cmake_minimum_required(VERSION 3.3)
project(p2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )

set(SOURCE_FILES
        p2_code/hw2_helper.cpp
        p2_code/hw2_helper.h p2_code/p2_main.cpp)

add_executable(p2 ${SOURCE_FILES})

target_link_libraries( p2 ${OpenCV_LIBS} )
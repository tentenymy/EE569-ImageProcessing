cmake_minimum_required(VERSION 3.3)
project(p1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )

set(SOURCE_FILES
        p1_code/classifier.cpp
        p1_code/classifier.h
    p1_code/hw2_helper.cpp
    p1_code/hw2_helper.h
        p1_code/p1_main.cpp
    )

add_executable(p1 ${SOURCE_FILES})

target_link_libraries( p1 ${OpenCV_LIBS} )

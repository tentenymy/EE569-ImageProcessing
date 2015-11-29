cmake_minimum_required(VERSION 3.3)
project(p3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )

set(SOURCE_FILES
        p3_code/main.cpp
        )

add_executable(p3 ${SOURCE_FILES})

target_link_libraries( p3 ${OpenCV_LIBS} )
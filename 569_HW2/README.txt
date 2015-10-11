# EE569 Homework Assignment #2
# Date:     Oct 20, 2015
# Name:     Meiyi Yang
# ID:       6761-0405-85
# Email:    meiyiyan@usc.edu
#########################################################
# Compiled on OS X Yosemite Version 10.10.4
# Define compiler and linker in CMakeLists.txt
# 		cmake_minimum_required(VERSION 3.3)
# Coding in C++11
# Matlab R2015a
# OpenCV: 3.0 (https://github.com/Itseez/opencv.git)
#########################################################

# 1. Usage for compiling, linking and runing
# Compile, link and default run:
# make sure you are in the HW2 folder: three sub-folder p1, p2, p3

# Problem1:
# in HW2 root folder
cd p1
cmake .
make
./p1

-------------------------------------------------

# Problem2 a & b & d:
# in HW2 root folder
cd p2
cmake .
make
./p2

# Problem2 c:
# Matlab: make sure two scripts are run under three toolbox:
# image processing toolbox
# Piotr's Computer Vision Matlab Toolbox (https://github.com/pdollar/toolbox.git)
# Structured Edge Detection Toolbox V3.0 (https://github.com/pdollar/edges.git)
run prob2c.m

-------------------------------------------------

# Problem3

-------------------------------------------------

# Clean compiling files:
make clean

#########################################################

# 2. File list and explanation

# Problem1:
# Main function:
# 	p1/p1_code/p1_main.cpp
#
# Class Classifer:
# 	p1/p1_code/classifer.h
# 	p1/p1_code/classifer.cpp
# 
# Class ImgMatOperator (General class for whole HW2)
# 	p1/p1_code/hw2_helper.h
# 	p1/p1_code/hw2_helper.cpp
#
# Makefile:
#   CMakeLists.txt

-------------------------------------------------

# Problem2:
# Main function:
# 	p2/p2_code/p2_main.cpp
#
# Class EdgeDetector:
# 	p2/p2_code/edge_detector.h
# 	p2/p2_code/edge_detector.cpp
# 
# Class ImgMatOperator (General class for whole HW2)
# 	p2/p2_code/hw2_helper.h
# 	p2/p2_code/hw2_helper.cpp
#
# Makefile:
#   CMakeLists.txt
# 
# p2c: matlab code
# 	p2/p2_code_matlab/prob2c.m
# 	p2/p2_code_matlab/detector_SE.m

#########################################################
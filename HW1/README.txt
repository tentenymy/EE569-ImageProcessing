# EE569 Homework Assignment #1
# Date:     Sep 5, 2015
# Name:     Meiyi Yang
# ID:       6761-0405-85
# Email:    meiyiyan@usc.edu
#
# Compiled on OS X Yosemite Version 10.10.4
#
# Define compiler and linker in Makefile
# Coding in C++11


# 1. Usage for compiling, linking and runing

# Compile, link and default run:

make


# Problem 1: Image manipulation and interpolation:
# a.	Image resizing via bilinear interpolation (10%)
	@echo "****** Problem 1.a ******"
	@g++ -c hw1_helper.cpp
	@g++ -c p1_resize.cpp
	@g++ -o p1_resize p1_resize.o hw1_helper.o
	@echo "USAGE: ./p1_resize [inputImageName] [outputImageName] 3 512 512"
	@./p1_resize the_starry_night.raw output1_resize.raw 3 512 512

# b.	Demosaicing of Bayer-patterned color image


# 1.	Bilinear Demosaicing (10%)


# 2.	Malvar-He-Culter (MHC) linear image demosaicing (10%)





# Clean compiling files:

make clean


# 2. File list and explanation


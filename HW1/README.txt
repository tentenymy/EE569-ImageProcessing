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
	./p1_resize the_starry_night.raw p1_resize.raw 3 512 512

# b.	Demosaicing of Bayer-patterned color image
	@echo "****** Problem 1.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p1_bilinear.cpp
	g++ -c p1_MHC.cpp
	g++ -o p1_bilinear p1_bilinear.o hw1_helper.o
	g++ -o p1_MHC p1_MHC.o hw1_helper.o
	@echo "USAGE: ./p1_bilinear [inputImageName] [outputImageName] 1 [width] [height]"
	@echo "USAGE: ./p1_MHC [inputImageName] [outputImageName] 1 [width] [height]"
	./p1_bilinear parrot_CFA.raw p1_bilinear.raw 1 424 636
	./p1_MHC parrot_CFA.raw p1_MHC1.raw 1 424 636

# Problem 2: Histogram equalization and image filtering
# a.	Histogram equalization
	@echo "****** Problem 2.a ******"
	g++ -c hw1_helper.cpp
	g++ -c p2_transfer.cpp
	g++ -o p2_transfer p2_transfer hw1_helper.o
	@echo "USAGE: ./p2_transfer [inputImageName] [outputImageName] 3 [width] [height]"
	./p2_transfer jet.raw p2_transfer.raw 3 512 512



# Clean compiling files:

make clean


# 2. File list and explanation


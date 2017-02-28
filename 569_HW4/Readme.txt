# EE569 Homework Assignment #4
# Date:     Nov 29, 2015
# Name:     Meiyi Yang
# Email:    meiyiyan@usc.edu
#########################################################
# Compiled on OS X Yosemite Version 10.10.4
# Define compiler and linker in CMakeLists.txt
# 		cmake_minimum_required(VERSION 3.3)
# Coding in C++11
# MATLAB R2015b

#########################################################
################### Problem 1: OCR ######################
#########################################################
# 1. Usage for compiling, linking and running
# Problem 1: OCR
# Run it on terminal
# Code in C++11
# Root HW4/p1

cmake .
make
./p1

# Clean compiling files:
make clean


————————————————————————————————————————————
# 2. File List
CMakeLists.txt         # Makefile
p1_code/p1_main.cpp    # Main function
p1_code/OCR.h          # Class OCR
p1_code/OCR.cpp        # Class OCR
p1_code/image.h        # Class Image
p1_code/image.cpp      # Class Image
p1_code/Morphology.h   # Class Morphology
p1_code/Morphology.cpp # Class Morphology




#########################################################
############# Problem 2: Contour Modeling ###############
#########################################################
# 1. Usage for compiling, linking and running
******************************
# Problem 2a: Snake algorithm
# Run it on Matlab
# Code in Matlab
# Root: HW4/p2

addpath(genpath(pwd));

To run it with GUI 
   1. Type guide on the matlab prompt. 
   2. Click on "Go to Existing GUI" 
   3. Select the snk.fig file in the directory HW4/p2/p2_code/p2_Snake_Algorithm
   4. Click the green arrow at the top to launch the GUI

   Once the GUI has been launched, you can use snakes by 
   1. Click on "New Image" and load an input image. Samples image are provided. 
   2. Set the smoothing parameter "sigma" or leave it at its default value and click "Filter". This will smooth the image. 
   3. As soon as you click "Filter", cross hairs would appear and using them and left click of you mouse you can pick initial contour location on the image. A red circle would appear everywhere you click and in most cases you should click all the way around the object you want to segment. The last point must be picked using a right-click in order to stop matlab for asking for more points. 
   4. Set the various snake parameters (relative weights of energy terms in the snake objective function) or leave them with their default value and click "Iterate" button. The snake would appear and move as it converges to its low energy state.
******************************
# Problem 2b: Level-Set Algorithm
# Run it on Matlab
# Code in Matlab
# Root HW4/p2

addpath(genpath(pwd));
run prob2b.m

————————————————————————————————————————————
# 2. File List
p2_code/p2_Level_Set/prob2b.m       # Main script for prob2b
p2_code/p2_Level_Set/drlse_edge.m   # Helper function for level-set
p2_code/p2_Level_Set/level_set.m    # Helper function for level-set
p1_code/p2_Snake_Algorithm/         # GUI for snake algorithm
————————————————————————————————————————————
# 3. CogyRight
# Snake Algorithm Source Code:
# Snakes: Active Contour Models
# by Ritwik Kumar
# 02 Jul 2010
# http://www.mathworks.com/matlabcentral/fileexchange/28109-snakes--active-contour-models
# Copyright (c) Ritwik Kumar, Harvard University 2010 
#              www.seas.harvard.edu/~rkkumar
# This code implements “Snakes: Active Contour Models” by Kass, Witkin and Terzopolous incorporating Eline, Eedge and Eterm energy factors. 
# See the included report and the paper to learn more.

# Level Set Algorithm Source Code:
# Copyright (c) 2011, Chunming Li
# All rights reserved.
# http://www.mathworks.com/matlabcentral/fileexchange/12711-level-set-for-image-segmentation





###########################################################################
######## Problem 3: Salient Point Descritors and Image Matching ###########
###########################################################################
# 1. Usage for compiling, linking and running
******************************
# Problem 3: Image Matching
# Problem 3a: Extraction and Descrition of Salient Points
# Run it on Matlab
# Code in Matlab
# Root HW4/p3

addpath(genpath(pwd));
run prob3a.m
******************************
# Problem 3b: Image Matching
# Run it on Matlab
# Code in Matlab
# Root HW4/p3

addpath(genpath(pwd));
run prob3b.m
******************************
# Problem 3c: Bag of Word
# Run it on Matlab
# Code in Matlab
# Root HW4/p3
# Parallel Computing Toolbox Required

addpath(genpath(pwd));
cd p3_code/BOW
compile
run prob3c.m
————————————————————————————————————————————
# 2. File List
p3_code/prob3a.m      # main script for problem3a
p3_code/prob3b.m      # main script for problem3b
p3_code/prob3c.m      # main script for problem3c
p3_code/SIFT/         # SIFT Source code
p3_code/SURF/         # SURF Source code
p3_code/BOW/          # BOW Source code
————————————————————————————————————————————
# 3. CogyRight
# SIFT Source Code:
# in HW4/p3/p3_code/SIFT
# Copyright (c) 2013, vijay anand
# All rights reserved.
# http://www.mathworks.com/matlabcentral/fileexchange/43724-sift--scale-invariant-feature-transform-?s_tid=srchtitle

# SURF Source code:
# in HW4/p3/p3_code/SURF
# Copyright (c) 2010, Dirk-Jan Kroon
# All rights reserved.
# http://www.mathworks.com/matlabcentral/fileexchange/28300-opensurf--including-image-warp-?s_tid=srchtitle

# BoW Source Code:
# in HW4/p3/p3_code/BOW
# Feature Extraction Toolbox for Computer Vision
# Copyright (c) 2013 Aditya Khosla#
# http://www.mathworks.com/matlabcentral/fileexchange/50448-computer-vision-feature-extraction-toolbox?s_tid=srchtitle











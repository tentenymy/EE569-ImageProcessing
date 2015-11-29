# EE569 Homework Assignment #4
# Date:     Nov 29, 2015
# Name:     Meiyi Yang
# ID:       6761-0405-85
# Email:    meiyiyan@usc.edu
#########################################################
# Compiled on OS X Yosemite Version 10.10.4
# Define compiler and linker in CMakeLists.txt
# 		cmake_minimum_required(VERSION 3.3)
# Coding in C++11
#########################################################
# 1. Usage for compiling, linking and running
# Compile, link and default run:
# Problem1:
# in HW4 root folder:
# Use cmake
cd p1
cmake .
make
./p1
——————————————————————
# Problem 2:
Snakes: Active Contour Models
by Ritwik Kumar
02 Jul 2010
http://www.mathworks.com/matlabcentral/fileexchange/28109-snakes--active-contour-models

To run it with GUI 
   1. Type guide on the matlab prompt. 
   2. Click on "Go to Existing GUI" 
   3. Select the snk.fig file in the same directory as this file 
   4. Click the green arrow at the top to launch the GUI

   Once the GUI has been launched, you can use snakes by 
   1. Click on "New Image" and load an input image. Samples image are provided. 
   2. Set the smoothing parameter "sigma" or leave it at its default value and click "Filter". This will smooth the image. 
   3. As soon as you click "Filter", cross hairs would appear and using them and left click of you mouse you can pick initial contour location on the image. A red circle would appear everywhere you click and in most cases you should click all the way around the object you want to segment. The last point must be picked using a right-click in order to stop matlab for asking for more points. 
   4. Set the various snake parameters (relative weights of energy terms in the snake objective function) or leave them with their default value and click "Iterate" button. The snake would appear and move as it converges to its low energy state.

Copyright (c) Ritwik Kumar, Harvard University 2010 
              www.seas.harvard.edu/~rkkumar

This code implements “Snakes: Active Contour Models” by Kass, Witkin and Terzopolous incorporating Eline, Eedge and Eterm energy factors. See the included report and the paper to learn more.

——————————————————————
# Clean compiling files:
make clean
#########################################################
 * 2. File list and explanation
 * Problem1. Geometrical Modification
 #########################################################
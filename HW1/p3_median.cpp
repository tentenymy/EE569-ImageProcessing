// EE569 Homework Assignment #1
// Date: Sep 12, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters and helper function

// Command


int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    Info info(argc, argv);
    Image image_old [info.height][info.width][info.byteperpixel];
    Image *pt_image_old = info.Info_File_Read();
    memcpy(image_old, pt_image_old, sizeof(image_old));

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 3. Image resizing via bilinear interpolation
    // Description.: resize a 512 * 512 image to 650 * 650 using image manipulation.

    // Algorithm: Bilinear interpolation
    // Description: XX
    // Time: O(3*m*n)

    // Explanation:

    // Result:


    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////

    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    info.Info_File_Write(pt_image_old);
    return 0;
}


// Helper function

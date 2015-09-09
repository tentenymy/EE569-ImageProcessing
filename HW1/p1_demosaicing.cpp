// EE569 Homework Assignment #1
// Date: Sep 08, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters:
// width must equal to height
// only resize to 650
const int size_new = 650;


int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    // Info info("the_starry_night.raw", "output_p1a.raw", 3, 512, 512);
    Info info(argc, argv);
    info.Info_File_Read();
    unsigned char image_data_old [info.width][info.height][info.byteperpixel];
    fread(image_data_old, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();

    info.Info_Print();
    //Image_Print_By_Interger(&image_data_old[0][0][0], &info, "image_print_by_interger.txt");
    Image_Plot_Gray_Line(&image_data_old[0][0][0], &info, "image_plot_gray_line.txt");

    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // Problem 1b.
    // Description:
    // Algorithm:
    // Time:

    // Explanation:



    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////


    // End.Write image data from image data matrix
    /*info.Info_File_Write();
    fwrite(image_data_new, sizeof(unsigned char), (size_t)size_new * size_new * info.byteperpixel, info.file);
    info.Info_File_Close();*/
    return 0;
}



// EE569 Homework Assignment #1
// Date: Sep 08, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters:



int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    // Info info("parrot_CFA.raw", "output_p1b.raw", 1, 424, 636);
    Info info(argc, argv);
    info.Info_File_Read();
    unsigned char imagedata_old [info.height][info.width][info.byteperpixel];
    fread(imagedata_old, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();

    info.Info_Print();
    //Image_Print_By_Interger(&imagedata_old[0][0][0], &info, "image_print_by_interger.txt");
    //Image_Plot_Gray_Line(&imagedata_old[0][0][0], &info, "image_plot_gray_line.txt");

    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // Problem 1b. Demosaicing of Bayer-patterned color image
    // Description : Demosaicing a Bayer-pattered gray image to a three-color image by both bilinear demosaicing
    // and MHC linear image demosaicing

    // Algorithm: Bilinear demosaicing; MHC (Malvar-He-Cutler) linear image demosaicing
    // Description: To get three colors in each pixel of the digital camera, the camera have to put three sensors in each pixel.
    // To avoid this wasting, the digital camera only puts one sensor in each pixel by Bayer Array to catch one color each.
    // And then use demosaicing algorithm to translate the Bayer Array to a three-color image.
    // There are two algorithms we need to implementation in this problem: bilinear demosaicing and MHC linear image demosaicing

    // Bilinear demosaicing:
    // For each pixel, we already know one color value.
    // The value of other two colors need to be estimated by their own nearest pixels of the same color.
    // For green color in Bayer Array, it has four neighbor pixels of green color.
    // For red or blue color in Bayer Array, they have two neighbor pixels of the same color each.
    // Combine R, G, B three color of one pixel together to generate a colorful image.

    // MHC linear image demosaicing

    // Time:

    // Explanation:



    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////
    info.byteperpixel = COLOR_BYTE;
    int add_boundary = 2;
    unsigned char imagedata_addboundary [info.height+ 2 * add_boundary] [info.width + 2 * add_boundary] [info.byteperpixel];
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                imagedata_addboundary[i + add_boundary][j + add_boundary][k] = imagedata_old[i][j][0];
        }
    }
    for (int j = add_boundary; j < info.width + add_boundary; j++)
    {
        for (int i = 0; i < add_boundary; i++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                imagedata_addboundary[add_boundary - 1 - i][j][k]
                        = imagedata_addboundary[add_boundary + 1 + i][j][k];
        }
        for (int i = 0; i < add_boundary; i++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                imagedata_addboundary[add_boundary + info.height + i][j][k]
                        = imagedata_addboundary[add_boundary + info.height - 2 - i][j][k];
        }
    }
    for (int i = 0; i < info.height + 2 * add_boundary; i++)
    {
        for (int j = 0; j < add_boundary; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                imagedata_addboundary[i][add_boundary - 1 - j][k]
                        = imagedata_addboundary[i][add_boundary + 1 + j][k];

        }
        for (int j = 0; j < add_boundary; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                imagedata_addboundary[i][add_boundary + info.width + j][k]
                        = imagedata_addboundary[i][add_boundary + info.width - 2 - j][k];
        }
    }


    info.width += 2 * add_boundary;
    info.height += 2 * add_boundary;


    int temp_i;
    int temp_j;
    // For red points
    for (int i = 1; 2 * i < info.height - add_boundary; i++)
    {
        for (int j = 1; 2 * j < info.width - add_boundary; j++)
        {
            // blue channel
            temp_i = 2 * i;
            temp_j = 2 * j;
            imagedata_addboundary[temp_i][temp_j][BLUE]
                    = (unsigned char)(0.25 * (double)(imagedata_addboundary[temp_i - 1][temp_j- 1][BLUE]
                                                      + imagedata_addboundary[temp_i + 1][temp_j - 1][BLUE]
                                                      + imagedata_addboundary[temp_i - 1][temp_j + 1][BLUE]
                                                      + imagedata_addboundary[temp_i + 1][temp_j + 1][BLUE]));

            // green channel
            imagedata_addboundary[temp_i][temp_j][GREEN]
                    = (unsigned char)(0.25 * (double)(imagedata_addboundary[temp_i - 1][temp_j][GREEN]
                                                      + imagedata_addboundary[temp_i + 1][temp_j][GREEN]
                                                      + imagedata_addboundary[temp_i][temp_j - 1][GREEN]
                                                      + imagedata_addboundary[temp_i][temp_j + 1][GREEN]));
        }
    }

    // For green points
    for (int i = 1; 2 * i + 1 < info.height - add_boundary; i++)
    {
        for (int j = 1; 2 * j + 1 < info.width - add_boundary; j++)
        {
            // blue channel
            temp_i = 2 * i + 1;
            temp_j = 2 * j;
            imagedata_addboundary[temp_i][temp_j][BLUE]
                    = (unsigned char)(0.5 * (double)(imagedata_addboundary[temp_i][temp_j - 1][BLUE]
                                                      + imagedata_addboundary[temp_i][temp_j + 1][BLUE]));
            // redchannel
            imagedata_addboundary[temp_i][temp_j][RED]
                    = (unsigned char)(0.5 * (double)(imagedata_addboundary[temp_i + 1][temp_j][RED]
                                                      + imagedata_addboundary[temp_i - 1][temp_j][RED]));

            temp_i = 2 * i;
            temp_j = 2 * j + 1;
            // blue channel
            imagedata_addboundary[temp_i][temp_j][BLUE]
                    = (unsigned char)(0.5 * (double)(imagedata_addboundary[temp_i - 1][temp_j][BLUE]
                                                     + imagedata_addboundary[temp_i + 1][temp_j][BLUE]));
            // redchannel
            imagedata_addboundary[temp_i][temp_j][RED]
                    = (unsigned char)(0.5 * (double)(imagedata_addboundary[temp_i][temp_j - 1][RED]
                                                     + imagedata_addboundary[temp_i][temp_j + 1][RED]));
        }
    }
    // For blue points
    for (int i = 1; 2 * i + 1 < info.height - add_boundary; i++)
    {
        for (int j = 1; 2 * j + 1 < info.width - add_boundary; j++)
        {
            // red channel
            temp_i = 2 * i + 1;
            temp_j = 2 * j + 1;
            imagedata_addboundary[temp_i][temp_j][RED]
                    = (unsigned char)(0.25 * (double)(imagedata_addboundary[temp_i - 1][temp_j- 1][RED]
                                                      + imagedata_addboundary[temp_i + 1][temp_j - 1][RED]
                                                      + imagedata_addboundary[temp_i - 1][temp_j + 1][RED]
                                                      + imagedata_addboundary[temp_i + 1][temp_j + 1][RED]));
            // green channel
            imagedata_addboundary[temp_i][temp_j][GREEN]
                    = (unsigned char)(0.25 * (double)(imagedata_addboundary[temp_i - 1][temp_j][GREEN]
                                                      + imagedata_addboundary[temp_i + 1][temp_j][GREEN]
                                                      + imagedata_addboundary[temp_i][temp_j - 1][GREEN]
                                                      + imagedata_addboundary[temp_i][temp_j + 1][GREEN]));
        }
    }




    // Test
    //makeinfo.Info_Print();
    Image_Print_By_Interger(&imagedata_addboundary[0][0][0], &info, "image_print_by_interger2.txt");

    // End.Write image data from image data matrix
    info.Info_File_Write();
    fwrite(imagedata_addboundary, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();
    return 0;
}



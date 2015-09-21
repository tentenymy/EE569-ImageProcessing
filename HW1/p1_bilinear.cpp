// EE569 Homework Assignment #1
// Date:     Sep 20, 2015
// Name:     Meiyi Yang
// ID:       6761-0405-85
// Email:    meiyiyan@usc.edu

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"

using namespace std;

// Set parameters and function
Image* Delete_Boundary (Image *, Info *, int);

// Command
// ./p1_bilinear parrot_CFA.raw p1_bilinear.raw 1 424 636

int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    Info info(argc, argv);
    Image image_old [info.height][info.width][info.byteperpixel];
    Image *pt_image_old = info.Info_File_Read();
    memcpy(image_old, pt_image_old, sizeof(image_old));

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 1b. Demosaicing of Bayer-patterned color image
    // Algorithm: Bilinear demosaicing
    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // 1. add 2-line boundary around the image
    info.byteperpixel = COLOR_BYTE;
    int add_boundary = 2;
    Image image_addboundary [info.height+ 2 * add_boundary] [info.width + 2 * add_boundary] [info.byteperpixel];

    // 1.1 get the value of original image to the center of the new image.
    // This new image is a color image.
    // Each color has the same value now.
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                image_addboundary[i + add_boundary][j + add_boundary][k] = image_old[i][j][0];
        }
    }
    // 1.2 add top and bottom boundary which like a mirror of original image.
    for (int j = add_boundary; j < info.width + add_boundary; j++)
    {
        for (int i = 0; i < add_boundary; i++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                image_addboundary[add_boundary - 1 - i][j][k]
                        = image_addboundary[add_boundary + 1 + i][j][k];
        }
        for (int i = 0; i < add_boundary; i++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                image_addboundary[add_boundary + info.height + i][j][k]
                        = image_addboundary[add_boundary + info.height - 2 - i][j][k];
        }
    }
    // 1.3 add right and left boundary which like a mirror of original image.
    for (int i = 0; i < info.height + 2 * add_boundary; i++)
    {
        for (int j = 0; j < add_boundary; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                image_addboundary[i][add_boundary - 1 - j][k]
                        = image_addboundary[i][add_boundary + 1 + j][k];

        }
        for (int j = 0; j < add_boundary; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                image_addboundary[i][add_boundary + info.width + j][k]
                        = image_addboundary[i][add_boundary + info.width - 2 - j][k];
        }
    }
    info.width += 2 * add_boundary;
    info.height += 2 * add_boundary;

    // 2. To estimate the other two colors in each point by bilinear demosaicing.
    int temp_i;
    int temp_j;
    // 2.1 For red points
    for (int i = 1; 2 * i < info.height - add_boundary; i++)
    {
        for (int j = 1; 2 * j < info.width - add_boundary; j++)
        {
            // blue channel
            temp_i = 2 * i;
            temp_j = 2 * j;
            image_addboundary[temp_i][temp_j][BLUE]
                    = (Image)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j- 1][BLUE]
                                                            + image_addboundary[temp_i + 1][temp_j - 1][BLUE]
                                                            + image_addboundary[temp_i - 1][temp_j + 1][BLUE]
                                                            + image_addboundary[temp_i + 1][temp_j + 1][BLUE]));
            // green channel
            image_addboundary[temp_i][temp_j][GREEN]
                    = (Image)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j][GREEN]
                                                            + image_addboundary[temp_i + 1][temp_j][GREEN]
                                                            + image_addboundary[temp_i][temp_j - 1][GREEN]
                                                            + image_addboundary[temp_i][temp_j + 1][GREEN]));
        }
    }
    // 2.2 For green points
    for (int i = 1; 2 * i + 1 < info.height - add_boundary; i++)
    {
        for (int j = 1; 2 * j + 1 < info.width - add_boundary; j++)
        {
            // blue channel
            temp_i = 2 * i + 1;
            temp_j = 2 * j;
            image_addboundary[temp_i][temp_j][BLUE]
                    = (Image)(0.5 * (double)(image_addboundary[temp_i][temp_j - 1][BLUE]
                                                           + image_addboundary[temp_i][temp_j + 1][BLUE]));
            // redchannel
            image_addboundary[temp_i][temp_j][RED]
                    = (Image)(0.5 * (double)(image_addboundary[temp_i + 1][temp_j][RED]
                                                           + image_addboundary[temp_i - 1][temp_j][RED]));

            temp_i = 2 * i;
            temp_j = 2 * j + 1;
            // blue channel
            image_addboundary[temp_i][temp_j][BLUE]
                    = (Image)(0.5 * (double)(image_addboundary[temp_i - 1][temp_j][BLUE]
                                                           + image_addboundary[temp_i + 1][temp_j][BLUE]));
            // redchannel
            image_addboundary[temp_i][temp_j][RED]
                    = (Image)(0.5 * (double)(image_addboundary[temp_i][temp_j - 1][RED]
                                                           + image_addboundary[temp_i][temp_j + 1][RED]));
        }
    }
    // 2.3 For blue points
    for (int i = 1; 2 * i + 1 < info.height - add_boundary; i++)
    {
        for (int j = 1; 2 * j + 1 < info.width - add_boundary; j++)
        {
            // red channel
            temp_i = 2 * i + 1;
            temp_j = 2 * j + 1;
            image_addboundary[temp_i][temp_j][RED]
                    = (Image)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j- 1][RED]
                                                            + image_addboundary[temp_i + 1][temp_j - 1][RED]
                                                            + image_addboundary[temp_i - 1][temp_j + 1][RED]
                                                            + image_addboundary[temp_i + 1][temp_j + 1][RED]));
            // green channel
            image_addboundary[temp_i][temp_j][GREEN]
                    = (Image)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j][GREEN]
                                                            + image_addboundary[temp_i + 1][temp_j][GREEN]
                                                            + image_addboundary[temp_i][temp_j - 1][GREEN]
                                                            + image_addboundary[temp_i][temp_j + 1][GREEN]));
        }
    }

    // 3. delete 2-line boundary to get a new output image
    Image *pt_image_output = Delete_Boundary(&image_addboundary[0][0][0], &info, add_boundary);
    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    info.Info_File_Write(pt_image_output);
    return 0;
}


// Delete the extra lines of an image
Image* Delete_Boundary (Image *pt_input, Info *pt_info, int add_boundary)
{
    Image image_input[pt_info->height] [pt_info->width] [pt_info->byteperpixel];
    memcpy(image_input, pt_input, sizeof(image_input));

    pt_info->height -= 2 * add_boundary;
    pt_info->width -= 2 * add_boundary;

    Image image_output[pt_info->height] [pt_info->width] [pt_info->byteperpixel];
    Image *pt_output = &image_output[0][0][0];
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
        {
            for (int k = 0; k < pt_info->byteperpixel; k++)
                image_output[i][j][k] = image_input[i + add_boundary][j + add_boundary][k];
        }
    }
    return pt_output;
}
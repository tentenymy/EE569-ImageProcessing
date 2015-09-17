// EE569 Homework Assignment #1
// Date: Sep 08, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"

using namespace std;

// Set parameters and function

int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    // Info info("parrot_CFA.raw", "output_p1b.raw", 1, 424, 636);
    Info info(argc, argv);
    info.Info_File_Read();
    unsigned char image_old [info.height][info.width][info.byteperpixel];
    fread(image_old, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();

    //Image_Print_By_Interger(&image_old[0][0][0], &info, "image_print_by_interger.txt");
    //Image_Plot_Gray_Line(&image_old[0][0][0], &info, "image_plot_gray_line.txt");

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 1b. Demosaicing of Bayer-patterned color image
    // Description : Demosaicing a Bayer-pattered gray image to a three-color image by both bilinear demosaicing
    // and MHC linear image demosaicing

    // Algorithm: Bilinear demosaicing;
    //            MHC (Malvar-He-Cutler) linear image demosaicing
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
    // Time: O(m * n)

    // Result:
    // This image is a little blur the color edge when I use photoshop to view it.
    // I think it is because it use an average calculation so the color of each pixel is similar.
    // It smooths the edges when the referenced points have a big difference between each other.




    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // 1. add 2-line boundary around the image
    info.byteperpixel = COLOR_BYTE;
    int add_boundary = 2;
    unsigned char image_addboundary [info.height+ 2 * add_boundary] [info.width + 2 * add_boundary] [info.byteperpixel];

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
                    = (unsigned char)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j- 1][BLUE]
                                                            + image_addboundary[temp_i + 1][temp_j - 1][BLUE]
                                                            + image_addboundary[temp_i - 1][temp_j + 1][BLUE]
                                                            + image_addboundary[temp_i + 1][temp_j + 1][BLUE]));
            // green channel
            image_addboundary[temp_i][temp_j][GREEN]
                    = (unsigned char)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j][GREEN]
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
                    = (unsigned char)(0.5 * (double)(image_addboundary[temp_i][temp_j - 1][BLUE]
                                                           + image_addboundary[temp_i][temp_j + 1][BLUE]));
            // redchannel
            image_addboundary[temp_i][temp_j][RED]
                    = (unsigned char)(0.5 * (double)(image_addboundary[temp_i + 1][temp_j][RED]
                                                           + image_addboundary[temp_i - 1][temp_j][RED]));

            temp_i = 2 * i;
            temp_j = 2 * j + 1;
            // blue channel
            image_addboundary[temp_i][temp_j][BLUE]
                    = (unsigned char)(0.5 * (double)(image_addboundary[temp_i - 1][temp_j][BLUE]
                                                           + image_addboundary[temp_i + 1][temp_j][BLUE]));
            // redchannel
            image_addboundary[temp_i][temp_j][RED]
                    = (unsigned char)(0.5 * (double)(image_addboundary[temp_i][temp_j - 1][RED]
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
                    = (unsigned char)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j- 1][RED]
                                                            + image_addboundary[temp_i + 1][temp_j - 1][RED]
                                                            + image_addboundary[temp_i - 1][temp_j + 1][RED]
                                                            + image_addboundary[temp_i + 1][temp_j + 1][RED]));
            // green channel
            image_addboundary[temp_i][temp_j][GREEN]
                    = (unsigned char)(0.25 * (double)(image_addboundary[temp_i - 1][temp_j][GREEN]
                                                            + image_addboundary[temp_i + 1][temp_j][GREEN]
                                                            + image_addboundary[temp_i][temp_j - 1][GREEN]
                                                            + image_addboundary[temp_i][temp_j + 1][GREEN]));
        }
    }

    // 3. delete 2-line boundary to get a new output image
    unsigned char *pt_image_output = Delete_Boundary(&image_addboundary[0][0][0], &info, add_boundary);
    unsigned char image_output[info.height] [info.width] [info.byteperpixel];
    memcpy(image_output, pt_image_output, sizeof(image_output));

    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    info.Info_File_Write();
    fwrite(image_output, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();
    return 0;
}

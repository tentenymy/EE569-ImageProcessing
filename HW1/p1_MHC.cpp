// EE569 Homework Assignment #1
// Date: Sep 08, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters or helper function
double Bound_Color(double);


int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    // Info info("parrot_CFA.raw", "output_p1b.raw", 1, 424, 636);
    Info info(argc, argv);
    info.Info_File_Read();
    unsigned char imagedata_old [info.height][info.width][info.byteperpixel];
    fread(imagedata_old, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();

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

    // MHC linear image demosaicing
    // Compared to the Bilinear demosaicing, MHC use additional points to evaluate the color.
    // The evaluation function still have the average of the neighbor points which have the same color we are being calculate.
    // Also have a weighted sum of points which have the same local color.
    // The weighted values is positive for the point which is being evaluated and is negative for the points which are nearby.
    // Different colors in this case have different weighted parameter.

    // Result:
    // Compared to the Bilinear demosaicing, the result of MHC is sharper.
    // By adding the correction term, we get more details in the image when the colors change quick in the small area.
    // But there is a special case in MHC.
    // The color value of one pixel may be out of the range [0, 255] during the calculation.
    // The maximum of these value may even large to 270, 280 or more, and I have to bound them to 0 or 255.
    // I think this is where we could try to improving in the future because we still lose some information.


    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // 1. add 2-line boundary around the image
    info.byteperpixel = COLOR_BYTE;
    int add_boundary = 2;
    unsigned char imagedata_addboundary [info.height+ 2 * add_boundary] [info.width + 2 * add_boundary] [info.byteperpixel];

    // 1.1 get the value of original image to the center of the new image.
    // This new image is a color image.
    // Each color has the same value now.
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                imagedata_addboundary[i + add_boundary][j + add_boundary][k] = imagedata_old[i][j][0];
        }
    }
    // 1.2 add top and bottom boundary which like a mirror of original image.
    // For example, not copy the edge line of the original image [column = 0]
    // Add a new line up the first line which is the same as the value in the column 1 of the original image
    // Add a second line up the line we added which has the same value of the column 2 of the original image.
    // Repeat this in the bottom of the image.
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
    // 1.3 add right and left boundary which like a mirror of original image.
    // The similar procedure as 1.2
    // The only difference between 1.2 is
    // we only mirror the original image by the first left, right line of the original image
    // we also mirror some value which we have added in the 1.2.
    // These points are all in the four corner of the image, totally 16 point.
    // We will use these in both demosaicing algorithm.
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

    // 2. To estimate the other two colors in each point by MHC demosaicing.
    int temp_i;
    int temp_j;
    double temp_sum;
    // 2.1 For red points
    for (int i = 1; 2 * i < info.height - add_boundary; i++)
    {
        for (int j = 1; 2 * j < info.width - add_boundary; j++)
        {
            // blue channel
            temp_i = 2 * i;
            temp_j = 2 * j;
            temp_sum = (0.125 * (2.0 * imagedata_addboundary[temp_i - 1][temp_j - 1][BLUE]
                         + 2 * imagedata_addboundary[temp_i + 1][temp_j - 1][BLUE]
                         + 2 * imagedata_addboundary[temp_i - 1][temp_j + 1][BLUE]
                         + 2 * imagedata_addboundary[temp_i + 1][temp_j + 1][BLUE]
                         + 6 * imagedata_addboundary[temp_i][temp_j][RED]
                         - 1.5 * imagedata_addboundary[temp_i - 2][temp_j][RED]
                         - 1.5 * imagedata_addboundary[temp_i + 2][temp_j][RED]
                         - 1.5 * imagedata_addboundary[temp_i][temp_j - 2][RED]
                         - 1.5 * imagedata_addboundary[temp_i][temp_j + 2][RED]));
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][BLUE] = (unsigned char)temp_sum;

            // green channel
            temp_sum = (0.125 * (2.0 * imagedata_addboundary[temp_i - 1][temp_j][GREEN]
                         + 2 * imagedata_addboundary[temp_i + 1][temp_j][GREEN]
                         + 2 * imagedata_addboundary[temp_i][temp_j - 1][GREEN]
                         + 2 * imagedata_addboundary[temp_i][temp_j + 1][GREEN]
                         + 4 * imagedata_addboundary[temp_i][temp_j][RED]
                         - imagedata_addboundary[temp_i - 2][temp_j][RED]
                         - imagedata_addboundary[temp_i + 2][temp_j][RED]
                         - imagedata_addboundary[temp_i][temp_j - 2][RED]
                         - imagedata_addboundary[temp_i][temp_j + 2][RED]));
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][GREEN] = (unsigned char)temp_sum;

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
            temp_sum = (0.125 * (4.0 * imagedata_addboundary[temp_i][temp_j - 1][BLUE]
                         + 4 * imagedata_addboundary[temp_i][temp_j + 1][BLUE]
                         + 5 * imagedata_addboundary[temp_i][temp_j][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i + 2][temp_j][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i - 2][temp_j][GREEN]
                         - imagedata_addboundary[temp_i][temp_j + 2][GREEN]
                         - imagedata_addboundary[temp_i][temp_j - 2][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j + 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j + 1][GREEN]));
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][BLUE] = (unsigned char)temp_sum;

            // red channel
            temp_sum = (0.125 * (4.0 * imagedata_addboundary[temp_i - 1][temp_j][RED]
                         + 4 * imagedata_addboundary[temp_i + 1][temp_j][RED]
                         + 5 * imagedata_addboundary[temp_i][temp_j][GREEN]
                         - imagedata_addboundary[temp_i + 2][temp_j][GREEN]
                         - imagedata_addboundary[temp_i - 2][temp_j][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i][temp_j + 2][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i][temp_j - 2][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j + 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j + 1][GREEN]));
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][RED] = (unsigned char)temp_sum;

            temp_i = 2 * i;
            temp_j = 2 * j + 1;
            // blue channel
            temp_sum = (0.125 * (4.0 * imagedata_addboundary[temp_i - 1][temp_j][BLUE]
                         + 4 * imagedata_addboundary[temp_i + 1][temp_j][BLUE]
                         + 5 * imagedata_addboundary[temp_i][temp_j][GREEN]
                         - imagedata_addboundary[temp_i + 2][temp_j][GREEN]
                         - imagedata_addboundary[temp_i - 2][temp_j][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i][temp_j + 2][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i][temp_j - 2][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j + 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j + 1][GREEN]));
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][BLUE] = (unsigned char)temp_sum;
            // red channel
            temp_sum = (0.125 * (4.0 * imagedata_addboundary[temp_i][temp_j + 1][RED]
                         + 4 * imagedata_addboundary[temp_i][temp_j - 1][RED]
                         + 5 * imagedata_addboundary[temp_i][temp_j][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i + 2][temp_j][GREEN]
                         + 0.5 * imagedata_addboundary[temp_i - 2][temp_j][GREEN]
                         - imagedata_addboundary[temp_i][temp_j + 2][GREEN]
                         - imagedata_addboundary[temp_i][temp_j - 2][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j - 1][GREEN]
                         - imagedata_addboundary[temp_i - 1][temp_j + 1][GREEN]
                         - imagedata_addboundary[temp_i + 1][temp_j + 1][GREEN]));
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][RED] = (unsigned char)temp_sum;
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
            temp_sum = 0.125 * (2.0 * imagedata_addboundary[temp_i - 1][temp_j - 1][RED]
                   + 2 * imagedata_addboundary[temp_i - 1][temp_j + 1][RED]
                   + 2 * imagedata_addboundary[temp_i + 1][temp_j - 1][RED]
                   + 2 * imagedata_addboundary[temp_i + 1][temp_j + 1][RED]
                   + 6 * imagedata_addboundary[temp_i][temp_j][BLUE]
                   - 1.5 * imagedata_addboundary[temp_i - 2][temp_j][BLUE]
                   - 1.5 * imagedata_addboundary[temp_i + 2][temp_j][BLUE]
                   - 1.5 * imagedata_addboundary[temp_i][temp_j - 2][BLUE]
                   - 1.5 * imagedata_addboundary[temp_i][temp_j + 2][BLUE]);
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][RED] = (unsigned char)temp_sum;

            // green channel
            temp_sum= 0.125 * (2.0 * imagedata_addboundary[temp_i - 1][temp_j][GREEN]
                   + 2 * imagedata_addboundary[temp_i + 1][temp_j][GREEN]
                   + 2 * imagedata_addboundary[temp_i][temp_j - 1][GREEN]
                   + 2 * imagedata_addboundary[temp_i][temp_j + 1][GREEN]
                   + 4 * imagedata_addboundary[temp_i][temp_j][BLUE]
                   - imagedata_addboundary[temp_i - 2][temp_j][BLUE]
                   - imagedata_addboundary[temp_i + 2][temp_j][BLUE]
                   - imagedata_addboundary[temp_i][temp_j - 2][BLUE]
                   - imagedata_addboundary[temp_i][temp_j + 2][BLUE]);
            temp_sum = Bound_Color(temp_sum);
            imagedata_addboundary[temp_i][temp_j][GREEN] = (unsigned char)temp_sum;


        }
    }

    // 3. delete 2-line boundary to get a new output image
    info.height -= 2 * add_boundary;
    info.width -= 2 * add_boundary;
    unsigned char imagedata_output [info.height] [info.width] [info.byteperpixel];
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
                imagedata_output[i][j][k] = imagedata_addboundary[i + add_boundary][j + add_boundary][k];
        }
    }

    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////
    // End.Write image data from image data matrix
    info.Info_File_Write();
    fwrite(imagedata_output, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();
    return 0;
}


double Bound_Color(double temp_sum)
{
    if (temp_sum > 255)
        temp_sum = 255;
    if (temp_sum < 0)
        temp_sum = 0;
    return temp_sum;
}
// EE569 Homework Assignment #1
// Date:     Sep 20, 2015
// Name:     Meiyi Yang
// ID:       6761-0405-85
// Email:    meiyiyan@usc.edu

#include <stdio.h>
#include <iostream>
#include "hw2_helper.h"
using namespace std;

// Set parameters and helper function
const int size_new = 650;

// Command
// ./p1_resize the_starry_night.raw p1_resize.raw 3 512 512

int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    Info info(argc, argv);
    Image image_old [info.height][info.width][info.byteperpixel];
    Image *pt_image_old = info.Info_File_Read();
    memcpy(image_old, pt_image_old, sizeof(image_old));

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 1a. Image resizing via bilinear interpolation
    // Algorithm: Bilinear interpolation
    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // 1. Generate a 650 * 650 * 3 empty array
    Image image_new [size_new][size_new][info.byteperpixel];

    const int size_old = info.width;
    double ratio = (double)(size_old - 1) / (double)(size_new - 1);

    double point_estimate_x;
    double point_estimate_y;
    int point_lt_x; //left top
    int point_lt_y;
    double scale_a;
    double scale_b;
    int point_rb_x; //right buttom
    int point_rb_y;

    for(int i = 0; i < size_new - 1; i++)
    {
        for(int j = 0; j < size_new - 1; j++)
        {
            // 2. Scale 650 * 650 to 512 * 512 by ratio (512 - 1)/ ( 650 - 1)
            point_estimate_x = ratio * (double)i;
            point_estimate_y = ratio *  (double)j;

            // 3. Find the nearest 4 points in the original 512*512 image of the estimated point
            // and get a and b (distance between estimated point to the top left point
            point_lt_x = (int)point_estimate_x;
            point_lt_y = (int)point_estimate_y;
            scale_a = point_estimate_x - (double)point_lt_x;
            scale_b = point_estimate_y - (double)point_lt_y;

            // 4. Calculate each estimated point by weighted the value of four nearest points
            // Calculate 649 * 649 points Matrix[0->648][0->648] first
            for(int k = 0; k < info.byteperpixel; k++)
            {
                double value = (1.0 - scale_a) * (1.0 - scale_b) * (double)image_old[point_lt_x][point_lt_y][k]
                               + (1.0 - scale_a) * scale_b * (double)image_old[point_lt_x][point_lt_y + 1][k]
                               + (1.0 - scale_b) * scale_a * (double)image_old[point_lt_x + 1][point_lt_y][k]
                               + scale_a * scale_b * (double)image_old[point_lt_x + 1][point_lt_y + 1][k];
                image_new[i][j][k] = (Image)round(value);
            }
        }
    }

    // 5. Repeat 2 -> 4. Calculate Matrix [0->648][649]
    for (int i = 0; i < size_new - 1; i++)
    {
        point_estimate_x = ratio * (double)i;
        point_rb_x = (int)point_estimate_x + 1;
        scale_a = 1.0  - (double)point_rb_x + point_estimate_x;
        for(int k = 0; k < info.byteperpixel; k++)
        {
            double value = (1.0 - scale_a) * (double)image_old[point_rb_x - 1][size_old - 1][k]
                           + scale_a * (double)image_old[point_rb_x][size_old - 1][k];
            image_new[i][size_new - 1][k] = (Image)round(value);
        }
    }

    // 6. Repeat 2 -> 4. Calculate Matrix [649][0->648]
    for (int j = 0; j < size_new - 1; j++)
    {
        point_estimate_y = ratio * (double)j;
        point_rb_y = (int)point_estimate_y + 1;
        scale_b = 1.0 + point_estimate_y - (double)point_rb_y;
        for(int k = 0; k < info.byteperpixel; k++)
        {
            double value = (1.0 - scale_b) * (double)image_old[size_old - 1][point_rb_y - 1][k]
                           + scale_b * (double)image_old[size_old - 1][point_rb_y][k];
            image_new[size_new - 1][j][k] = (Image)round(value);
        }
    }

    // 7. Calculate Matrix [649][649]
    for(int k = 0; k < info.byteperpixel; k++)
    {
        image_new[size_new - 1][size_new - 1][k] = image_old[size_old - 1][size_old - 1][k];
    }

    // 8. Set info class to write file
    info.width = size_new;
    info.height = size_new;
    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    info.Info_File_Write(&image_new[0][0][0]);
    return 0;
}



// EE569 Homework Assignment #1
// Date: Sep 06, 2015
// Name: Meiyi Yang
// Problem 1.a Image resizing via bilinear interpolation

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

    Image_Plot_All_Line(&image_data_old[0][0][0], &info, "image_plot_all_line.txt");
    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // Problem 1a. Image resizing via bilinear interpolation
    // Description: resize a 512 * 512 image to 650 * 650 using image manipulation.
    // Algorithm: Bilinear interpolation
    // Time: O(3*m*n)

    // Explanation:
    // if the nearest four points of the estimated point are more than four.
    // I assume the estimated point is on the top and left of its four referenced points as the first order.
    // For instance, the estimated point (A, B) is on the original point.
    // Then the four nearest points are (A, B) (A, B+1), (A+1, B), (A+1, B+1)
    // Or (A, B) is on the line between (A-0.1,B) and (A+0.9,B).
    // Then the four nearest points are (A-0.1,B), (A+0.9,B), (A-0.1, B+1), (A+0.9, B+1)
    // If the estimated point has no right or bottom point.
    // In this case, those special estimated points are the right and bottom boundary.
    // Then use the left or top point of the estimated point.


    // 1. Generate a 650 * 650 * 3 empty array
    unsigned char image_data_new [size_new][size_new][info.byteperpixel];

    const int size_old = info.width;
    double ratio = (double)(size_old - 1) / (double)(size_new - 1);

    double d_estimate_point_x;
    double d_estimate_point_y;
    int i_left_top_point_x;
    int i_left_top_point_y;
    double d_scale_a;
    double d_scale_b;
    int i_right_buttom_point_x;
    int i_right_buttom_point_y;

    for(int i = 0; i < size_new - 1; i++)
    {
        for(int j = 0; j < size_new - 1; j++)
        {
            // 2. Scale 650 * 650 to 512 * 512 by ratio (512 - 1)/ ( 650 - 1)
            d_estimate_point_x = ratio * (double)i;
            d_estimate_point_y = ratio *  (double)j;

            // 3. Find the nearest 4 points in the original 512*512 image of the estimated point
            // and get a and b (distance between estimated point to the top left point
            i_left_top_point_x = (int)d_estimate_point_x;
            i_left_top_point_y = (int)d_estimate_point_y;
            d_scale_a = d_estimate_point_x - (double)i_left_top_point_x;
            d_scale_b = d_estimate_point_y - (double)i_left_top_point_y;

            // 4. Calculate each estimated point by weighted the value of four nearest points
            // Calculate 649 * 649 points Matrix[0->648][0->648] first
            for(int k = 0; k < info.byteperpixel; k++)
            {
                double value = (1.0 - d_scale_a) * (1.0 - d_scale_b) * (double)image_data_old[i_left_top_point_x][i_left_top_point_y][k]
                               + (1.0 - d_scale_a) * d_scale_b * (double)image_data_old[i_left_top_point_x][i_left_top_point_y + 1][k]
                               + (1.0 - d_scale_b) * d_scale_a * (double)image_data_old[i_left_top_point_x + 1][i_left_top_point_y][k]
                               + d_scale_a * d_scale_b * (double)image_data_old[i_left_top_point_x + 1][i_left_top_point_y + 1][k];
                image_data_new[i][j][k] = (unsigned char)round(value);
            }
        }
    }

    // 5. Repeat 2 -> 4. Calculate Matrix [0->648][649]
    for (int i = 0; i < size_new - 1; i++)
    {
        d_estimate_point_x = ratio * (double)i;
        i_right_buttom_point_x = (int)d_estimate_point_x + 1;
        d_scale_a = 1.0  - (double)i_right_buttom_point_x + d_estimate_point_x;
        for(int k = 0; k < info.byteperpixel; k++)
        {
            double value = (1.0 - d_scale_a) * (double)image_data_old[i_right_buttom_point_x - 1][size_old - 1][k]
                           + d_scale_a * (double)image_data_old[i_right_buttom_point_x][size_old - 1][k];
            image_data_new[i][size_new - 1][k] = (unsigned char)round(value);
        }
    }

    // 6. Repeat 2 -> 4. Calculate Matrix [649][0->648]
    for (int j = 0; j < size_new - 1; j++)
    {
        d_estimate_point_y = ratio * (double)j;
        i_right_buttom_point_y = (int)d_estimate_point_y + 1;
        d_scale_b = 1.0 + d_estimate_point_y - (double)i_right_buttom_point_y;
        for(int k = 0; k < info.byteperpixel; k++)
        {
            double value = (1.0 - d_scale_b) * (double)image_data_old[size_old - 1][i_right_buttom_point_y - 1][k]
                           + d_scale_b * (double)image_data_old[size_old - 1][i_right_buttom_point_y][k];
            image_data_new[size_new - 1][j][k] = (unsigned char)round(value);
        }
    }

    // 7. Calculate Matrix [649][649]
    for(int k = 0; k < info.byteperpixel; k++)
    {
        image_data_new[size_new - 1][size_new - 1][k] = image_data_old[size_old - 1][size_old - 1][k];
    }

    // 8. Set info class to write file
    info.width = size_new;
    info.height = size_new;
    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////


    // End.Write image data from image data matrix
    info.Info_File_Write();
    fwrite(image_data_new, sizeof(unsigned char), (size_t)size_new * size_new * info.byteperpixel, info.file);
    info.Info_File_Close();
    return 0;
}



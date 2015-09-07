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

    //Info info("the_starry_night.raw", "output_p1a.raw", 3, 512, 512);
    Info info(argc, argv);
    info.Info_File_Read();
    unsigned char image_data_old [info.width][info.height][info.byteperpixel];
    fread(image_data_old, sizeof(unsigned char), (size_t)info.width * info.height * info.byteperpixel, info.file);
    info.Info_File_Close();



    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    const int size_old = info.width;
    double ratio = (double)(size_old - 1) / (double)(size_new - 1);
    unsigned char image_data_new [size_new][size_new][info.byteperpixel];

    for(int i = 0; i < size_new; i++)
    {
        for(int j = 0; j < size_new; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                image_data_new[i][j][k] = (unsigned char)0;
            }
        }
    }

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
            d_estimate_point_x = ratio * (double)i;
            d_estimate_point_y = ratio *  (double)j;
            i_left_top_point_x = (int)d_estimate_point_x;
            i_left_top_point_y = (int)d_estimate_point_y;
            d_scale_a = d_estimate_point_x - (double)i_left_top_point_x;
            d_scale_b = d_estimate_point_y - (double)i_left_top_point_y;
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

    for(int k = 0; k < info.byteperpixel; k++)
    {
        image_data_new[size_new - 1][size_new - 1][k] = image_data_old[size_old - 1][size_old - 1][k];
    }

    info.width = size_new;
    info.height = size_new;

    // Write image data from image data matrix
    info.Info_File_Write();
    fwrite(image_data_new, sizeof(unsigned char), (size_t)size_new * size_new * info.byteperpixel, info.file);
    info.Info_File_Close();
    info.Info_Print();


    // Test
    //Image_Print_By_Interger(&image_data_new[0][0][0], &info, "image_print_by_interger.txt");
    //Image_Plot_All_Line(&image_data_new[0][0][0], &info, "image_plot_all_line2.txt");
    return 0;
}



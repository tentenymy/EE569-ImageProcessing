// EE569 Homework Assignment #1
// Date: Sep 17, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters and helper function
const double PI  = 3.141592653;

Image* Filter_Gaussian(Image*, Info*, int, double);
void Find_Window(int, int, Info*, int*, int);
double Bound_Color(double);
void Get_PSNR(Image *, Image *p, Info *);

// Command


int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    Info info(argc, argv);

    char filename_ori[] = "peppers.raw";
    Image image_ori [info.height][info.width][info.byteperpixel];
    Image *pt_image_ori = info.Info_File_Read(filename_ori);
    memcpy(image_ori, pt_image_ori, sizeof(image_ori));

    char filename_noisy[] = "peppers_noisy.raw";
    Image image_noisy [info.height][info.width][info.byteperpixel];
    Image *pt_image_noisy = info.Info_File_Read(filename_noisy);
    memcpy(image_noisy, pt_image_noisy, sizeof(image_noisy));

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 1a. Image resizing via bilinear interpolation
    // Description.: resize a 512 * 512 image to 650 * 650 using image manipulation.
    // Algorithm: Bilinear interpolation
    // Description: XX
    // Time: O(3*m*n)
    // Explanation:
    // Result:
    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    Image *pt_image_new = NULL;

    cout << "0, 0" << endl;
    Get_PSNR(&image_noisy[0][0][0], &image_ori[0][0][0], &info);

    cout << "G 3, 1.8" << endl;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, 3, 1.8);
    char filename1[] = "p2_filter_G3.raw";
    info.Info_File_Write(pt_image_new, filename1);
    Get_PSNR(pt_image_new, &image_ori[0][0][0], &info);

    cout << "G 5, 1.8" << endl;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, 5, 1.8);
    char filename2[] = "p2_filter_G5.raw";
    info.Info_File_Write(pt_image_new, filename2);
    Get_PSNR(pt_image_new, &image_ori[0][0][0], &info);
    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    //info.Info_File_Write(pt_image_new);
    return 0;
}


// Helper function
Image* Filter_Gaussian(Image *pt_image, Info *pt_info, int window_size, double sigma)
{
    // Set image
    Image image[pt_info->height][pt_info->width][pt_info->byteperpixel];
    Image image_new[pt_info->height][pt_info->width][pt_info->byteperpixel];
    Image *pt_image_new = &image_new[0][0][0];
    memcpy(image, pt_image, sizeof(image));

    // Set Gaussian template
    int center_size = window_size / 2;
    double template_gaussian[window_size * window_size];
    double template_total = 0;

    for (int i = 0; i < window_size; i++)
    {
        for (int j = 0; j < window_size; j++)
        {
            double temp = (i - center_size) * (i - center_size) + (j - center_size) * (j - center_size);
            double temp2 = exp(-temp / (2 * sigma * sigma));
            template_gaussian[i * window_size + j] = temp2 / (2 * PI * sigma * sigma);
            template_total += template_gaussian[i * window_size + j];
        }
    }

    for (int i = 0; i < window_size * window_size; i++)
    {
        template_gaussian[i] /= template_total;

    }

    // Apply Gaussian filter to the image
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
        {
            int window[4];
            Find_Window(i, j, pt_info, window, window_size);
            for (int k = 0; k < pt_info->byteperpixel; k++)
            {
                double window_count = 0;
                double window_total = 0;
                double window_weight = 0;
                for (int m = window[0]; m <= window[2]; m++)
                {
                    for (int n = window[1]; n <= window[3]; n++)
                    {
                        window_weight = template_gaussian[(center_size + m - i) * window_size + (center_size + n - j)];
                        window_count += window_weight;
                        window_total += window_weight * image[m][n][k];
                    }
                }
                image_new[i][j][k] = (int)Bound_Color(window_total/window_count);
            }
        }

    }
    return pt_image_new;
}

void Find_Window(int x, int y, Info* pt_info, int* window, int window_size)
{
    int boundary = window_size / 2;
    window[0] = x - boundary;
    window[1] = y - boundary;
    window[2] = x + boundary;
    window[3] = y + boundary;
    if (window[0] < 0)
        window[0] = 0;
    if (window[1] < 0)
        window[1] = 0;
    if (window[2] > pt_info->height)
        window[2] = pt_info->height;
    if (window[3] > pt_info->width)
        window[3] = pt_info->width;
    //cout << "WINDOW:(" << x << "," << y << ")" << window[0] << " " <<  window[1] << " " <<  window[2] << " " <<  window[3] << endl;
}

double Bound_Color(double temp_sum)
{
    if (temp_sum > 255)
        temp_sum = 255;
    if (temp_sum < 0)
        temp_sum = 0;
    return temp_sum;
}

void Get_PSNR(Image *pt_image_new, Image *pt_image_old, Info *pt_info)
{

    //Image image_noisy[pt_info->height][pt_info->width][pt_info->byteperpixel];
    //Image image_noisy[pt_info->height][pt_info->width][pt_info->byteperpixel];

    double psnr[3];
    double mse[3] = {};
    int total;
    int intensity_old, intensity_new;
    int size = pt_info->height * pt_info->width;


    for (int i = 0; i < size; i++)
    {
        intensity_old = (int)*pt_image_old++;
        intensity_new = (int)*pt_image_new++;
        mse[0] += (intensity_old - intensity_new) * (intensity_old - intensity_new) / (double)size;

        intensity_old = (int)*pt_image_old++;
        intensity_new = (int)*pt_image_new++;
        mse[1] += (intensity_old - intensity_new) * (intensity_old - intensity_new) / (double)size;

        intensity_old = (int)*pt_image_old++;
        intensity_new = (int)*pt_image_new++;
        mse[2] += (intensity_old - intensity_new) * (intensity_old - intensity_new) / (double)size;
        //cout << intensity_old << " " << intensity_new << endl;
        //cout << "MSE: " << mse[0] << " " << mse[1] << " " << mse[2] << endl;
    }


    psnr[0] = 10.0 * log10(255 * 255.0 / mse[0]);
    psnr[1] = 10.0 * log10(255 * 255.0 / mse[1]);
    psnr[2] = 10.0 * log10(255 * 255.0 / mse[2]);
    cout << "MSE: " << mse[0] << " " << mse[1] << " " << mse[2] << endl;
    cout << "PSNR:" << psnr[0] << " " << psnr[1] << " " << psnr[2] << endl;

}
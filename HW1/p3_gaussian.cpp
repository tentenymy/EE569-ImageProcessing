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
Image* Filter_Median(Image*, Info*, int, bool isupdate);
void Find_Window(int, int, Info*, int*, int);
double Bound_Color(double);
void Print_PSNR(Image *, Image *p, Info *, int, double);
int Get_median(int *, int);

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
    Image *pt_image_new = &image_noisy[0][0][0];
    int window_size = 0;
    double sigma = 0;
    char filename[30];

    Print_PSNR(&image_noisy[0][0][0], &image_ori[0][0][0], &info, window_size, sigma);

    window_size = 3;
    sigma = 1.2;

    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    strcpy(filename, "p3_filter_MM.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = &image_noisy[0][0][0];
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    strcpy(filename, "p3_filter_GG.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = &image_noisy[0][0][0];
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    strcpy(filename, "p3_filter_MG.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = &image_noisy[0][0][0];
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    strcpy(filename, "p3_filter_GM.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    /*pt_image_new = &image_noisy[0][0][0];
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    strcpy(filename, "p3_filter_MMG.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = &image_noisy[0][0][0];
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    strcpy(filename, "p3_filter_MGG.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = &image_noisy[0][0][0];
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    pt_image_new = Filter_Median(pt_image_new, &info, window_size, false);
    strcpy(filename, "p3_filter_MMM.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = &image_noisy[0][0][0];
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    pt_image_new = Filter_Gaussian(pt_image_new, &info, window_size, sigma);
    strcpy(filename, "p3_filter_GGG.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);*/



    /*window_size = 3;
    pt_image_new = Filter_Median(&image_noisy[0][0][0], &info, window_size, false);
    strcpy(filename, "p3_filter_M3.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = Filter_Median(&image_noisy[0][0][0], &info, window_size, true);
    strcpy(filename, "p3_filter_M3_update.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    window_size = 5;
    pt_image_new = Filter_Median(&image_noisy[0][0][0], &info, window_size, false);
    strcpy(filename, "p3_filter_M5.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    pt_image_new = Filter_Median(&image_noisy[0][0][0], &info, window_size, true);
    strcpy(filename, "p3_filter_M5_update.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);*/


    /*window_size = 3;
    sigma = 0.5;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G3_05.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    sigma = 0.8;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G3_08.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    sigma = 1.2;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G3_12.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    sigma = 1.8;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G3_18.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);


    window_size = 5;
    sigma = 0.5;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G5_05.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    sigma = 0.8;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G5_08.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    sigma = 1.2;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G5_12.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);

    sigma = 1.8;
    pt_image_new = Filter_Gaussian(&image_noisy[0][0][0], &info, window_size, sigma);
    strcpy(filename, "p3_filter_G5_18.raw");
    info.Info_File_Write(pt_image_new, filename);
    Print_PSNR(pt_image_new, &image_ori[0][0][0], &info, window_size, sigma);*/
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
            double temp3 = exp(-temp / (2 * sigma * sigma));
            template_gaussian[i * window_size + j] = temp3 / (2 * PI * sigma * sigma);
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

void Print_PSNR(Image *pt_image_new, Image *pt_image_old, Info *pt_info, int window_size, double sigma)
{

    //Image image_noisy[pt_info->height][pt_info->width][pt_info->byteperpixel];
    //Image image_noisy[pt_info->height][pt_info->width][pt_info->byteperpixel];

    double psnr[3];
    double mse[3] = {};

    double average;

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
    }


    psnr[0] = 10.0 * log10(255 * 255.0 / mse[0]);
    psnr[1] = 10.0 * log10(255 * 255.0 / mse[1]);
    psnr[2] = 10.0 * log10(255 * 255.0 / mse[2]);

    average = (mse[0] + mse[1] + mse[2]) / 3;

    cout << "Window: " << window_size << "  sigma: " << sigma << endl;
    cout << "   MSE: " << mse[0] << " " << mse[1] << " " << mse[2] << " AVE: " << average << endl;
    //cout << "   PSNR:" << psnr[0] << " " << psnr[1] << " " << psnr[2] << endl;

}

Image * Filter_Median(Image *pt_image, Info *pt_info, int window_size, bool isupdate)
{
    // Set image
    Image image[pt_info->height][pt_info->width][pt_info->byteperpixel];
    Image image_new[pt_info->height][pt_info->width][pt_info->byteperpixel];
    Image *pt_image_new = &image_new[0][0][0];
    memcpy(image, pt_image, sizeof(image));

    // Set Gaussian template
    int center_size = window_size / 2;

    // Apply Gaussian filter to the image
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
        {
            int window[4];
            Find_Window(i, j, pt_info, window, window_size);
            for (int k = 0; k < pt_info->byteperpixel; k++)
            {
                int window_value[window_size * window_size];
                int median;
                int window_count = 0;
                for (int m = window[0]; m <= window[2]; m++)
                {
                    for (int n = window[1]; n <= window[3]; n++)
                    {
                        window_value[window_count] = (int)image[m][n][k];
                        window_count++;
                    }
                }
                median = Get_median(window_value, window_count);

                /*if (isupdate)
                {
                    if (image[i][j][k] == window_value[0] || image[i][j][k] == window_value[window_count - 1])
                        image_new[i][j][k] = median;
                    else
                        image_new[i][j][k] = image[i][j][k];
                }
                else*/
                    image_new[i][j][k] = median;
            }
        }

    }
    return pt_image_new;
}

int Get_median(int *window_value, int window_count)
{
    int temp;
    //int window_value[window_count];
    //memcpy(window_value, pt_window_value, sizeof(window_value));

    for (int i = 0; i < window_count - 1; i++)
    {
        for (int j = 0; j < window_count - 1 - i; j++)
        {
            if (window_value[j] > window_value[j + 1])
            {
                temp = window_value[j];
                window_value[j] = window_value[j + 1];
                window_value[j + 1] = temp;
            }
        }
    }
    return window_value[window_count/2];
}
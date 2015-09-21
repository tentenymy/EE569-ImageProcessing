// EE569 Homework Assignment #1
// Date: Sep 18, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters and helper function
const double PI  = 3.141592653;

void Find_Window(int, int, Info*, int*, int);
double Bound_Color(double);
void Print_PSNR(Image*, Image *p, Info *, int, double);
int* Filter_Box(int*, Info*, int);
double* Filter_Box(double *, Info *, int);
int Bound_Index(int, int);
int* Get_Mean(int*, int *, Info *, int);
double* Get_Mean(double*, int *, Info *, int);
// Command
Image* Filter_Guided(int*, Info*, int, double);


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
    /*Image *pt_image_new = &image_noisy[0][0][0];
    int window_size = 0;
    double sigma = 0;
    char filename[30];

    Print_PSNR(&image_noisy[0][0][0], &image_ori[0][0][0], &info, window_size, sigma);

    window_size = 3;
    sigma = 1.2;*/


    Image_Print_By_Interger(&image_noisy[0][0][0], &info, "image_print_by_interger.txt");

    // Set I, P, I * I, I * P
    int image_I_R[info.height][info.width];
    int image_I_G[info.height][info.width];
    int image_I_B[info.height][info.width];
    int image_P[info.height][info.width];
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            image_I_R[i][j] = (int)image_noisy[i][j][RED];
            image_I_G[i][j] = (int)image_noisy[i][j][GREEN];
            image_I_B[i][j] = (int)image_noisy[i][j][BLUE];
        }
    }


    char filename[9][40];
    int radius[3] = {2, 4, 8};
    double eps[3] = {0.1, 0.2, 0.4};
    strcpy(filename[0], "p3_guided_0.raw");
    strcpy(filename[1], "p3_guided_1.raw");
    strcpy(filename[2], "p3_guided_1.raw");
    strcpy(filename[3], "p3_guided_1.raw");
    strcpy(filename[4], "p3_guided_1.raw");
    strcpy(filename[5], "p3_guided_1.raw");
    strcpy(filename[6], "p3_guided_1.raw");
    strcpy(filename[7], "p3_guided_1.raw");
    strcpy(filename[8], "p3_guided_1.raw");

    Image image_Q[info.height][info.width][info.byteperpixel];
    Image *image_q;


    for (int m = 0; m < 3; m++)
    {
        for (int n = 0; n < 3; n++)
        {
            image_q = Filter_Guided(&image_I_R[0][0], &info, radius[m], eps[n]);
            for (int i = 0; i < info.height; i++)
            {
                for (int j = 0; j < info.width; j++)
                    image_Q[i][j][RED] = *image_q++;
            }

            image_q = Filter_Guided(&image_I_G[0][0], &info, radius[m], eps[n]);
            for (int i = 0; i < info.height; i++)
            {
                for (int j = 0; j < info.width; j++)
                    image_Q[i][j][GREEN] = *image_q++;
            }

            image_q = Filter_Guided(&image_I_B[0][0], &info, radius[m], eps[n]);
            for (int i = 0; i < info.height; i++)
            {
                for (int j = 0; j < info.width; j++)
                    image_Q[i][j][BLUE] = *image_q++;
            }
            info.Info_File_Write(&image_Q[0][0][0], filename[m * 3 + n]);
            Print_PSNR(&image_Q[0][0][0], &image_ori[0][0][0], &info, radius[m], eps[n]);
        }
    }


    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    return 0;
}

Image* Filter_Guided(int *pt_image_I, Info *pt_info, int radius, double eps)
{
    // Set N
    int height = pt_info->height;
    int width = pt_info->width;
    int num_box[height][width];
    int size = sizeof(num_box);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            num_box[i][j] = 1;
    }
    memcpy(num_box, Filter_Box(&num_box[0][0], pt_info, radius), size);

    // Set mean(I)
    int image_I[height][width];
    memcpy(image_I, pt_image_I, sizeof(image_I));
    int mean_I[height][width];
    memcpy(mean_I, Get_Mean(pt_image_I, (int*)&num_box[0][0], pt_info, radius), size);

    // Set (I * P)
    int image_I_I[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            image_I_I[i][j] = image_I[i][j] * image_I[i][j];
    }

    // mean(I * P)
    int mean_I_I[height][width];
    memcpy(mean_I_I, Get_Mean(&image_I_I[0][0], (int*)&num_box[0][0], pt_info, radius), size);

    // cov(IP)
    int cov_I_I[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            cov_I_I[i][j] = mean_I_I[i][j] - mean_I[i][j] * mean_I[i][j];
    }

    // var(I)
    int var_I_I[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            var_I_I[i][j] = mean_I_I[i][j] - mean_I[i][j] * mean_I[i][j];
    }

    // a, b
    double para_a[height][width];
    double para_b[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            para_a[i][j] = cov_I_I[i][j] / (var_I_I[i][j] + eps);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            para_b[i][j] = mean_I[i][j] / (para_a[i][j] * mean_I[i][j]);
    }

    // mean_a, mean_b
    double mean_a[height][width];
    memcpy(mean_a, Get_Mean(&para_a[0][0], (int*)&num_box[0][0], pt_info, radius), sizeof(mean_a));
    double mean_b[height][width];
    memcpy(mean_b, Get_Mean(&para_b[0][0], (int*)&num_box[0][0], pt_info, radius), sizeof(mean_a));


    Image image_Q[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            image_Q[i][j] = mean_a[i][j] * image_I[i][j] + mean_b[i][j];
    }

    Image *pt_image_q = &image_Q[0][0];
    return pt_image_q;
}




// Helper function
double* Get_Mean(double *pt_image, int *pt_N, Info *pt_info, int radius)
{
    double *pt_sum = Filter_Box(pt_image, pt_info, radius);
    double temp_sum;
    double temp_N;
    double mean[pt_info->height][pt_info->width];

    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j <  pt_info->width; j++)
        {
            temp_sum = *pt_sum;
            temp_N = *pt_N;
            pt_sum++;
            pt_N++;
            mean[i][j] = temp_sum/temp_N;
        }

    }
    double *pt_mean = &mean[0][0];
    //cout << "MEAN: " << mean[500][500] << endl;
    return pt_mean;
}

int* Get_Mean(int *pt_image, int *pt_N, Info *pt_info, int radius)
{
    int *pt_sum = Filter_Box(pt_image, pt_info, radius);
    double temp;
    double temp_sum;
    double temp_N;
    int mean[pt_info->height][pt_info->width];

    //cout << "MEAN: " << mean[0][0] << endl;
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
        {
            temp_sum = *pt_sum;
            temp_N = *pt_N;
            pt_sum++;
            pt_N++;
            mean[i][j] = temp_sum/temp_N;
        }

    }
    int *pt_mean = &mean[0][0];
    return pt_mean;
}


// Helper function
int* Filter_Box(int *pt_image, Info *pt_info, int radius)
{
    int input[pt_info->height][pt_info->width];
    memcpy(input, pt_image, sizeof(input));
    int sum[pt_info->height][pt_info->width];


    // Initial
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
            sum[i][j] = 0;
    }

    // Horizontal
    int input_after, input_before;
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < radius + 1; j++)
        {
            sum[i][0] += (int)input[i][j];
        }

    }
    //cout << "1: " << sum[0][0] << endl;

    for(int i = 0; i < pt_info->height;i++)
    {
        for (int j = 1; j < pt_info->width; j++)
        {
            if (j + radius > pt_info->width - 1)
                input_after = 0;
            else
                input_after = (int)input[i][j + radius];
            if (j - radius - 1 < 0)
                input_before = 0;
            else
                input_before = (int)input[i][j - radius - 1];
            sum[i][j] = sum[i][j - 1] + input_after - input_before;
        }
    }
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
            input[i][j] = sum[i][j];
    }

    // Vertical
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 1; j < radius + 1; j++)
            sum[0][i] += sum[j][i];
    }
    //cout << "2: " << sum[0][0] << endl;

    for(int i = 1; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
        {
            if (i + radius >= pt_info->height)
                input_after = 0;
            else
                input_after = sum[i + radius][j];
            if (i - radius - 1 < 0)
                input_before = 0;
            else
                input_before = (int)input[i - radius - 1][j];
            sum[i][j] = sum[i - 1][j] + input_after - input_before;
        }
    }
    //cout << "3: " << sum[0][0] << endl;
    int *pt_sum = &sum[0][0];
    return pt_sum;
}

double* Filter_Box(double *pt_image, Info *pt_info, int radius)
{
    double input[pt_info->height][pt_info->width];
    memcpy(input, pt_image, sizeof(input));
    double sum[pt_info->height][pt_info->width];


    // Initial
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
            sum[i][j] = 0;
    }

    // Horizontal
    int input_after, input_before;
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < radius + 1; j++)
        {
            sum[i][0] += (double)input[i][j];
        }

    }
    //cout << "1: " << sum[0][0] << endl;

    for(int i = 0; i < pt_info->height;i++)
    {
        for (int j = 1; j < pt_info->width; j++)
        {
            if (j + radius > pt_info->width - 1)
                input_after = 0;
            else
                input_after = (double)input[i][j + radius];
            if (j - radius - 1 < 0)
                input_before = 0;
            else
                input_before = (double)input[i][j - radius - 1];
            sum[i][j] = sum[i][j - 1] + input_after - input_before;
        }
    }
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
            input[i][j] = sum[i][j];
    }

    // Vertical
    for (int i = 0; i < pt_info->height; i++)
    {
        for (int j = 1; j < radius + 1; j++)
            sum[0][i] += sum[j][i];
    }
    //cout << "2: " << sum[0][0] << endl;

    for(int i = 1; i < pt_info->height; i++)
    {
        for (int j = 0; j < pt_info->width; j++)
        {
            if (i + radius >= pt_info->height)
                input_after = 0;
            else
                input_after = sum[i + radius][j];
            if (i - radius - 1 < 0)
                input_before = 0;
            else
                input_before = (double)input[i - radius - 1][j];
            sum[i][j] = sum[i - 1][j] + input_after - input_before;
        }
    }
    //cout << "3: " << sum[0][0] << endl;
    double *pt_sum = &sum[0][0];
    return pt_sum;
}

int Bound_Index(int index, int bound)
{
    if (index > bound)
        index = bound;
    if (index < 0)
        index = 0;
    return index;
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









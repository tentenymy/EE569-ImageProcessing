// EE569 Homework Assignment #1
// Date: Sep 12, 2015
// Name: Meiyi Yang
// Problem 1.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters and helper function
int* Plot_Num_PDF(Image*, Info*);
int Find_Num_Bin(int *, int, int);
void Add_Frequency_Color(int*, int*, int, int, int);
void Find_Window(int, int, Info*o, int*, int);

const int NUM_BIN = 8;
const int WINDOW_SIZE = 11;

// Command
const string filename[3] = {"red.txt", "green.txt", "blue.txt"};
const string FILE_INDEX_CDF = "p2b_CDF_";
const string FILE_INDEX_PDF = "p2b_PDF_";
const string FILE_INDEX_NUM_PDF = "p2b_NUM_PDF_";
const string FILE_INDEX_METHOD = "p2b_TF_";

int main(int argc, char *argv[])
{
    // Begin.Read the input image data from file
    Info info(argc, argv);
    Image image_old [info.height][info.width][info.byteperpixel];
    Image *pt_image_old = info.Info_File_Read();
    memcpy(image_old, pt_image_old, sizeof(image_old));

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 2b. Image filtering - creating oil painting effect
    // Description.: create a oil-painting effect for images and discuss its filter parameter.

    // Algorithm: Image filtering for oil-painting effect
    // Description: 
    // First we decrease the number of each color intensity to 4,
    // by getting the mean of 4 same-number-pixel area in the color histogram of the image.
    // Then we use a N * N window to pick the most frequent color.
    // This frequent color is a combination of RGB image.

    // Explanation:

    // Result:
    
    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // Get histogram
    int channel[info.byteperpixel][info.width * info.height];
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            channel[RED][i * info.width + j] = (int)image_old[i][j][RED];
            channel[GREEN][i * info.width + j] = (int)image_old[i][j][GREEN];
            channel[BLUE][i * info.width + j] = (int)image_old[i][j][BLUE];
        }
    }

    int num_pdf[info.byteperpixel][COLOR_SIZE]; // 3 * 255
    int temp;

    int total_per_bin = info.width * info.height / NUM_BIN;
    int average[info.byteperpixel][NUM_BIN]; // 3 * 4
    int min_bin[info.byteperpixel * NUM_BIN]; // 3 * 4
    min_bin[0] = 0; // Red
    min_bin[NUM_BIN] = 0; // GREEN
    min_bin[NUM_BIN * 2] = 0; // BLUE

    for (int k = 0; k < info.byteperpixel; k++)
    {
        for (int i = 0; i < COLOR_SIZE; i++)
            num_pdf[k][i] = 0;
        for (int i = 0; i < info.width * info.height; i++)
        {
            temp = channel[k][i];
            num_pdf[k][temp]++;
        }

        // Divide into 4 bins
        int total_num = 0;
        int total_value = 0;
        int index_bin = 0;

        for (int i = 0; i < COLOR_SIZE; i++)
        {
            total_num += num_pdf[k][i];
            total_value += num_pdf[k][i] * i;
            if (total_num >= total_per_bin)
            {
                average[k][index_bin++] = total_value / total_num;
                min_bin[k * NUM_BIN + index_bin] = i + 1;
                total_num = 0;
                total_value = 0;
            }
        }
        average[k][index_bin++] = total_value / total_num;
    }


    // Decrease the color in the image
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
            {
                temp = image_old[i][j][k];
                int no_bin = Find_Num_Bin(min_bin, temp, k);
                image_old[i][j][k] = average[k][no_bin];
            }
        }
    }

    // Image filter by neighborhood
    Image image_new[info.height][info.width][info.byteperpixel];

    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            int max_freq_red, max_freq_green, max_freq_blue;
            int max_freq_val = 0;
            int min_freq_val = 200;
            int window[4];
            int histogram[NUM_BIN * NUM_BIN * NUM_BIN] = {};

            Find_Window(i, j, &info, window, WINDOW_SIZE);
            for (int m = window[0]; m < window[2]; m++)
            {
                for (int n = window[1]; n < window[3]; n++)
                {
                    Add_Frequency_Color(histogram, min_bin,
                                        image_old[m][n][RED], image_old[m][n][GREEN], image_old[m][n][BLUE]);
                }
            }

            for (int m = 0; m < NUM_BIN; m++)
            {
                for (int n = 0; n < NUM_BIN; n++)
                {
                    for (int k = 0; k < NUM_BIN; k++)
                    {
                        if (histogram[m * NUM_BIN * NUM_BIN + n * NUM_BIN + k] > max_freq_val)
                        {
                            max_freq_val = histogram[m * NUM_BIN * NUM_BIN + n * NUM_BIN + k];
                            max_freq_red = m;
                            max_freq_green = n;
                            max_freq_blue = k;
                        }
                    }

                }
            }
            image_new[i][j][RED] = average[RED][max_freq_red];
            image_new[i][j][GREEN] = average[GREEN][max_freq_green];
            image_new[i][j][BLUE] = average[BLUE][max_freq_blue];
        }
    }


    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    char filename[] = "p2_oilpaintint_nofilter.raw";
    info.Info_File_Write(&image_old[0][0][0], filename);

    Plot_Num_PDF(&image_old[0][0][0], &info);
    info.Info_File_Write(&image_new[0][0][0]);
    return 0;
}


// Helper function
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
}


void Add_Frequency_Color(int *histogram, int *min_bin, int color_red, int color_green, int color_blue)
{
    int index_color[COLOR_BYTE];
    index_color[RED] = Find_Num_Bin(min_bin, color_red, RED);
    index_color[GREEN] = Find_Num_Bin(min_bin, color_green, GREEN);
    index_color[BLUE] = Find_Num_Bin(min_bin, color_blue, BLUE);
    histogram[index_color[RED] * NUM_BIN * NUM_BIN + index_color[GREEN]* NUM_BIN  + index_color[BLUE]]++;
}


int Find_Num_Bin(int *min_bin, int find_val, int COLOR)
{
    for (int i = 1; i < NUM_BIN - 1; i++)
    {
        if (find_val < min_bin[COLOR * NUM_BIN + i])
            return (i - 1);
    }
    return NUM_BIN - 1;
}



int* Plot_Num_PDF(Image *pt_image, Info *pt_info)
{
    int channel[pt_info->byteperpixel][pt_info->width * pt_info->height];
    int pdf[pt_info->byteperpixel][COLOR_SIZE];
    ofstream fout;
    int temp;
    int *pt_pdf = &pdf[0][0];

    for (int i = 0; i < pt_info->width; i++)
    {
        for (int j = 0; j < pt_info->height; j++)
        {
            channel[RED][i * pt_info->height + j] = (int)*pt_image++;
            channel[GREEN][i * pt_info->height + j] = (int)*pt_image++;
            channel[BLUE][i * pt_info->height + j] = (int)*pt_image++;
        }
    }

    for (int k = 0; k < pt_info->byteperpixel; k++)
    {
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            pdf[k][i] = 0;
        }
        for (int i = 0; i < pt_info->width * pt_info->height; i++)
        {
            temp = channel[k][i];
            pdf[k][temp]++;
        }
        fout.open(FOLDER_WRITE + FILE_INDEX_PDF + filename[k]);
        for (int i = 0; i < COLOR_SIZE; i++)
            fout << pdf[k][i] << endl;
        fout.close();
    }

    return pt_pdf;
}
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


    ofstream fout;

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

    // PDF
    int num_pdf[info.byteperpixel][COLOR_SIZE]; // 3 * 255
    int temp;

    int num_bin = 4;
    int total_per_bin = info.width * info.height / num_bin;
    int average[info.byteperpixel][num_bin];
    int min_color_per_bin[info.byteperpixel][num_bin];

    cout << total_per_bin << endl;
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
        min_color_per_bin[k][0] = 0;

        for (int i = 0; i < COLOR_SIZE; i++)
        {
            total_num += num_pdf[k][i];
            total_value += num_pdf[k][i] * i;
            if (total_num >= total_per_bin)
            {
                average[k][index_bin++] = total_value / total_num;
                min_color_per_bin[k][index_bin] = i + 1;
                total_num = 0;
                total_value = 0;
                cout << i << endl;
            }
        }
        average[k][index_bin++] = total_value / total_num;
    }

    cout << endl;
    for (int k = 0; k < 3; k++)
    {
        for(int i = 0; i < 4; i++)
        {
            //cout << i << ": " << average[k][i] << endl;
        }
        cout << endl;
    }
    cout << endl;

    for (int k = 0; k < 3; k++)
    {
        for(int i = 0; i < 4; i++)
        {
            //cout << i << ": " << min_color_per_bin[k][i] << endl;
        }
        cout << endl;
    }

    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
            {
                temp = image_old[i][j][k];
                if (temp < min_color_per_bin[k][1])
                {
                    image_old[i][j][k] = average[k][0];
                }
                else if (temp < min_color_per_bin[k][2])
                {
                    image_old[i][j][k] = average[k][1];
                }
                else if (temp < min_color_per_bin[k][3])
                {
                    image_old[i][j][k] = average[k][2];
                }
                else
                {
                    image_old[i][j][k] = average[k][3];
                }
            }
        }
    }


    Plot_Num_PDF(&image_old[0][0][0], &info);
    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////

    // End.Write image data from image data matrix
    info.Info_File_Write(&image_old[0][0][0]);
    return 0;
}


// Helper function
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

        fout.open(FOLDER_WRITE + FILE_INDEX_NUM_PDF + filename[k]);
        for (int i = 0; i < COLOR_SIZE; i++)
            fout << pdf[k][i] << endl;
        fout.close();
    }
    return pt_pdf;
}
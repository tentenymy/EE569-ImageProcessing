// EE569 Homework Assignment #1
// Date:     Sep 20, 2015
// Name:     Meiyi Yang
// ID:       6761-0405-85
// Email:    meiyiyan@usc.edu

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters and helper function
const string filename[3] = {"red.txt", "green.txt", "blue.txt"};
const string FILE_INDEX_CDF = "p2a1_CDF_";
const string FILE_INDEX_PDF = "p2a1_PDF_";
const string FILE_INDEX_METHOD = "p2a1_TF_";

// Command
double* Plot_PDF(Image*, Info*);
double* Plot_CDF(Image*, Info*);

int main(int argc, char *argv[]) {
    // Begin.Read the input image data from file
    Info info(argc, argv);
    Image image_old[info.height][info.width][info.byteperpixel];
    Image *pt_image_old = info.Info_File_Read();
    memcpy(image_old, pt_image_old, sizeof(image_old));
    pt_image_old = &image_old[0][0][0];

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 2b. Histogram Equalization
    // Description.: Use histogram equalization methods to enhance the contrast

    // Algorithm: Transfer-function-based histogram equalization method
    //            Cumulative-probability-based histogram equalization method

    // Description: These two methods are all using histogram equalization to enhance the contrast.
    // The core idea is that by getting the CDF(Cumulative pdfbution Function) and PDF (Probability pdfbution Function) of each color of the image,
    // we can get the probability of each color value the image have from 0 to 255.
    // In most cases, especially for real image, the pdfbutions of each color value are not balanced.
    // And the range of color value may not exactly from 0 to 255, sometimes, the range is smaller.
    // So to enhance the contrast, we can normalize or equalize the pdfbution of each color of the image to make the range of the color larger
    // and to enhance the "important part" of the color intensity.


    // Transfer-function-based histogram equalization method
    // Description: Using normalized probability function to normalized the original CDF of each color channel in the image.
    // The formula is really easy to implementation.
    // For the intensity value x in the CDF of each color channel,
    // TF(x) = CDF(x) * 255;
    // x : the intensity in the CDF of each color channel
    // CDF(x): the cumulative probability of x
    // TF(x): the transfer function of x

    // Time: O(M * N)
    // Explanation:

    // Result: The enhancement result is more efficient than I thought.
    // A large number of details are revealed by applying the transfer function.
    // By plotting a PDF and CDF figure of each channel for the original image, I find that the range of each color is relatively concentrated.
    // But after the enhancement, the pdf of the image changed separately.


    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // 1. Get CDF and plot
    double *pt_cdf = Plot_CDF(&image_old[0][0][0], &info);
    double cdf[info.byteperpixel][COLOR_SIZE];
    memcpy(cdf, pt_cdf, sizeof(cdf));

    // 2. Generate the transfer function for each color
    short tranferfunc[info.byteperpixel][COLOR_SIZE];
    ofstream fout;
    int temp;
    for (int k = 0; k < info.byteperpixel; k++)
    {
        for (int i = 0; i < COLOR_SIZE; i++)
            tranferfunc[k][i] = (short)floor(cdf[k][i] * COLOR_SIZE);

        fout.open(FOLDER_WRITE + FILE_INDEX_METHOD + filename[k]);
        for (int i = 0; i < COLOR_SIZE; i++)
            fout << tranferfunc[k][i] << endl;
        fout.close();
    }


    // 3. Apply the transfer function to the image
    for (int i = 0; i < info.height; i++)
    {
        for (int j = 0; j < info.width; j++)
        {
            for (int k = 0; k < info.byteperpixel; k++)
            {
                temp = image_old[i][j][k];
                image_old[i][j][k] = tranferfunc[k][temp];
            }
        }
    }

    // 4. Plot pdf/cdf after the transfer function
    Plot_PDF(&image_old[0][0][0], &info);
    Plot_CDF(&image_old[0][0][0], &info);
    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////
    // End.Write image data from image data matrix
    info.Info_File_Write(pt_image_old);
    return 0;
}

// Helper function
double* Plot_PDF(Image *pt_image, Info *pt_info)
{
    short channel_new[pt_info->byteperpixel][pt_info->width * pt_info->height];
    double pdf[pt_info->byteperpixel][COLOR_SIZE];
    ofstream fout;
    int temp;
    double *pt_pdf = &pdf[0][0];

    for (int i = 0; i < pt_info->width; i++)
    {
        for (int j = 0; j < pt_info->height; j++)
        {
            channel_new[RED][i * pt_info->height + j] = (short)*pt_image++;
            channel_new[GREEN][i * pt_info->height + j] = (short)*pt_image++;
            channel_new[BLUE][i * pt_info->height + j] = (short)*pt_image++;
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
            temp = channel_new[k][i];
            pdf[k][temp]++;
        }
        for (int i = 0; i < COLOR_SIZE; i++)
            pdf[k][i] /= (double)pt_info->width * pt_info->height;

        fout.open(FOLDER_WRITE + FILE_INDEX_PDF + filename[k]);
        for (int i = 0; i < COLOR_SIZE; i++)
            fout << pdf[k][i] << endl;
        fout.close();
    }
    return pt_pdf;
}


double* Plot_CDF(Image *pt_image, Info *pt_info)
{
    short channel_new[pt_info->byteperpixel][pt_info->width * pt_info->height];
    double pdf[pt_info->byteperpixel][COLOR_SIZE];
    double cdf[pt_info->byteperpixel][COLOR_SIZE];
    ofstream fout;
    int temp;
    double *pt_cdf = &cdf[0][0];

    for (int i = 0; i < pt_info->width; i++)
    {
        for (int j = 0; j < pt_info->height; j++)
        {
            channel_new[RED][i * pt_info->height + j] = (short)*pt_image++;
            channel_new[GREEN][i * pt_info->height + j] = (short)*pt_image++;
            channel_new[BLUE][i * pt_info->height + j] = (short)*pt_image++;
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
            temp = channel_new[k][i];
            pdf[k][temp]++;
        }
        for (int i = 0; i < COLOR_SIZE; i++)
            pdf[k][i] /= (double)pt_info->width * pt_info->height;

        cdf[k][0] = pdf[k][0];
        fout.open(FOLDER_WRITE + FILE_INDEX_CDF + filename[k]);
        fout << cdf[k][0] << endl;
        for (int i = 1; i < COLOR_SIZE; i++)
        {
            cdf[k][i] = cdf[k][i - 1] + pdf[k][i];
            fout << cdf[k][i] << endl;
        }
        fout.close();
    }
    return pt_cdf;
}
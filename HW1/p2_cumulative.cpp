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
const string FILE_INDEX_CDF = "p2a2_CDF_";
const string FILE_INDEX_PDF = "p2a2_PDF_";
const string FILE_INDEX_METHOD = "p2a2_CF_";
const string FILE_INDEX_NUM_CDF = "p2a2_NUM_CDF_";

// Command
double* Plot_PDF(Image*, Info*);
double* Plot_CDF(Image*, Info*);
int* Plot_Num_CDF(Image*, Info*);

int main(int argc, char *argv[]) {
    // Begin.Read the input image data from file
    Info info(argc, argv);
    Image image[info.height][info.width][info.byteperpixel];
    Image *pt_image = info.Info_File_Read();
    memcpy(image, pt_image, sizeof(image));
    pt_image = &image[0][0][0];

    ///////////////////////////////////////// INSERT YOUR COMMENT HERE /////////////////////////////////////////
    // Problem 2b. Histogram Equalization
    // Description.: Use histogram equalization methods to enhance the contrast

    // Algorithm: Transfer-function-based histogram equalization method
    //            Cumulative-probability-based histogram equalization method

    // Description: These two methods are all using histogram equalization to enhance the contrast.
    // The core idea is that by getting the CDF(Cumulative distribution Function) and PDF (Probability pdfbution Function) of each color of the image,
    // we can get the probability of each color value the image have from 0 to 255.
    // In most cases, especially for real image, the distributions of each color value are not balanced.
    // And the range of color value may not exactly from 0 to 255, sometimes, the range is smaller.
    // So to enhance the contrast, we can normalize or equalize the distribution of each color of the image to make the range of the color larger
    // and to enhance the "important part" of the color intensity.


    // Cumulative-probability-based histogram equalization method
    // Description: this method enhances the image by change the pdf of each color intensity to the same.
    // It likes a bucket filling.
    // Here we have 255 color intensity (255 buckets) and 512 * 512 pixels (512 * 512 balls).
    // Method B make every color intensity has the same number of pixels, which is 512 * 512 / 255.
    // The order of putting balls is intensity and bucket number increasing.

    // Result: Compared to the transfer-function-based histogram equalization method,
    // this cumulative-probability-based method is harder to implement.
    // Cumulative-probability-based method gets a more equalized probability distribution,
    // which I thought would get a more efficient image.
    // But the results of these two methods are similar.
    // To try to analyse this, I plot both the PDF and CDF of enhanced images for two methods.
    // It is easy to find that there is only a slightly difference in CDF but a big difference in PDF.
    // In two PDF figures, these two methods have a similar range of color intensity,
    // but the probability and distribution of each intensity are not the same.
    // I think the equal probability of intensity the method B used leads to a result that
    // both details of the image and noise of the image increase.

    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // 1. Get pixel number CDF and plot
    int *pt_num_cdf = Plot_Num_CDF(&image[0][0][0], &info);
    int num_cdf[info.byteperpixel][COLOR_SIZE];
    memcpy(num_cdf, pt_num_cdf, sizeof(num_cdf));

    // 2. Generate the bucket function
    int pixel_total_num = info.width * info.height;
    int pixel_per_bucket = pixel_total_num / COLOR_SIZE;
    int bucket_map[pixel_total_num];
    for (int i = 0; i < pixel_total_num; i++)
        bucket_map[i] = floor(i / pixel_per_bucket);
    int bucket_num_map[info.byteperpixel][COLOR_SIZE]; // 3 * 255

    // 3. Generate the mapping function
    int index_color, index_map;
    for (int k = 0; k < info.byteperpixel; k++)
    {
        bucket_num_map[k][0] = 0;
        for (int i = 1; i < COLOR_SIZE; i++)
            bucket_num_map[k][i] = num_cdf[k][i - 1];

        for (int i = 0; i < info.width; i++)
        {
            for (int j = 0; j < info.height; j++)
            {
                index_color = image[i][j][k]; // 0-255
                index_map = bucket_num_map[k][index_color]++; // 0 - 512 * 512
                image[i][j][k] = bucket_map[index_map];
            }
        }
    }

    // 4. Plot pdf/cdf after the transfer function
    Plot_PDF(&image[0][0][0], &info);
    Plot_CDF(&image[0][0][0], &info);
    ////////////////////////////////////// PROCESSING CODE END //////////////////////////////////////
    // End.Write image data from image data matrix
    info.Info_File_Write(&image[0][0][0]);
    return 0;
}

// Helper function
double* Plot_PDF(Image *pt_image, Info *pt_info)
{
    short channel[pt_info->byteperpixel][pt_info->width * pt_info->height];
    double pdf[pt_info->byteperpixel][COLOR_SIZE];
    ofstream fout;
    int temp;
    double *pt_pdf = &pdf[0][0];

    for (int i = 0; i < pt_info->width; i++)
    {
        for (int j = 0; j < pt_info->height; j++)
        {
            channel[RED][i * pt_info->height + j] = (short)*pt_image++;
            channel[GREEN][i * pt_info->height + j] = (short)*pt_image++;
            channel[BLUE][i * pt_info->height + j] = (short)*pt_image++;
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
        for (int i = 0; i < COLOR_SIZE; i++)
            pdf[k][i] /= (double)pt_info->width * pt_info->height;

        fout.open(FOLDER_WRITE + FILE_INDEX_PDF + filename[k]);
        for (int i = 0; i < COLOR_SIZE; i++)
            fout << pdf[k][i] << endl;
        fout.close();
    }
    return pt_pdf;
}


int* Plot_Num_CDF(Image *pt_image, Info *pt_info)
{
    int channel[pt_info->byteperpixel][pt_info->width * pt_info->height];
    int pdf[pt_info->byteperpixel][COLOR_SIZE];
    int cdf[pt_info->byteperpixel][COLOR_SIZE];
    ofstream fout;
    int temp;
    int *pt_cdf = &cdf[0][0];

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

        cdf[k][0] = pdf[k][0];
        fout.open(FOLDER_WRITE + FILE_INDEX_NUM_CDF + filename[k]);
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
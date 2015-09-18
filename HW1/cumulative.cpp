// EE569 Homework Assignment #1
// Date: Sep 12, 2015
// Name: Meiyi Yang
// Problem 2.b

#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"
using namespace std;

// Set parameters and helper function
const string filename[3] = {"p2_plot_red.txt", "p2_plot_green.txt", "p2_plot_blue.txt"};
const string FILE_INDEX_CDF = "CDF";
const string FILE_INDEX_PDF = "PDF";
const string FILE_INDEX_METHOD = "TF";

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
    // The core idea is that by getting the CDF(Cumulative Distribution Function) and PDF (Probability pdfbution Function) of each color of the image,
    // we can get the probability of each color value the image have from 0 to 255.
    // In most cases, especially for real image, the pdfbutions of each color value are not balanced.
    // And the range of color value may not exactly from 0 to 255, sometimes, the range is smaller.
    // So to enhance the contrast, we can normalize or equalize the pdfbution of each color of the image to make the range of the color larger
    // and to enhance the "important part" of the color intensity.

    // Cumulative-probability-based histogram equalization method
    // Description: Differ from changing the distribution of CDF in the first method,
    // cumulative-probability-based equalization apply to the distribution of probability in each intensity (PDF)
    // The goal is that every color intensity has the same probability.

    // Time: O(M * N)
    // Explanation:

    // Result:



    ////////////////////////////////////// INSERT YOUR PROCESSING CODE HERE //////////////////////////////////////
    // 1. Get number of pixels in each intensity
    short channel_new[pt_info->byteperpixel][pt_info->width * pt_info->height];
    short num_pdf[pt_info->byteperpixel][COLOR_SIZE];
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
            num_pdf[k][i] = 0;
        }
        for (int i = 0; i < pt_info->width * pt_info->height; i++)
        {
            temp = channel_new[k][i];
            num_pdf[k][temp]++;
            cout << "i: " << i << "num: " << num_pdf[k][temp] << endl;
        }
    }

    // 2. Generate the Cumulative-probability-based function for each color
    short cumulative_func[info.byteperpixel][COLOR_SIZE];
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
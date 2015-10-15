/* EE569 Homework Assignment #2
 * Date: October 14, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * email: meiyiyan@usc.edu
 * Problem2. Image Segmentation
 *
 * Main function:
 * p2_main.cpp
 *
 * Class slic:
 * slic.h
 * slic.cpp
 *
 * Function Meanshift
 * meanshift.h
 * meanshift.cpp
 *
 * Class ImgMatOperator (General class for whole HW2)
 * hw2_helper.h
 * hw2_helper.cpp
 *
 * MATLAB code
 * p3b.m
 * p3c.m
 * run_cpp.m
 * run_evaluation.m
 */

/* slic.h.
 *
 * Written by: Pascal Mettes.
 *
 * This file contains the class elements of the class Slic. This class is an
 * implementation of the SLIC Superpixel algorithm by Achanta et al. [PAMI'12,
 * vol. 34, num. 11, pp. 2274-2282].
 *
 * This implementation is created for the specific purpose of creating
 * over-segmentations in an OpenCV-based environment.
 * From: https://github.com/PSMM/SLIC-Superpixels
 */

/* meanshift.h
 * Author: Kanglai Qian
 * From: https://github.com/qiankanglai/opencv.meanshift
 */


#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "opencv2/core/cuda.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <float.h>


#include "hw2_helper.h"
#include "slic.h"
#include "meanshift.h"

using namespace cv;
using namespace std;


const string FOLDER = "p3_image/";
const string LABLE1 = "Man";
const string LABLE2 = "Rhinos";

/*
 * @para
 * filename read : raw image data
 * filename_write: raw image write to
 * nr_superpixels: number of superpixels
 * nc: weight-factors
 */
void Run_Slic(string filename_read, string filename_write, int nr_superpixels, int nc, int height, int width, int byte) {
  /* Load the image and convert to Lab colour space. */
  Mat mat_original = ImgMatOperator::Mat_Raw_Read(filename_read, height, width, byte);
  IplImage image_original = mat_original;
  IplImage *lab_image = cvCloneImage(&image_original);
  cvCvtColor(&image_original, lab_image, CV_BGR2Lab);

  /* Yield the number of superpixels and weight-factors from the user. */
  int w = image_original.width, h = image_original.height;
  double step = sqrt((w * h) / (double) nr_superpixels);

  /* Perform the SLIC superpixel algorithm. */
  Slic slic;
  slic.generate_superpixels(lab_image, step, nc);
  slic.create_connectivity(lab_image);
  slic.display_contours(&image_original, CV_RGB(255,0,0));
  //slic.colour_with_cluster_means(&image_original);

  // Write to the file and show the image
  IplImage *image_result = &image_original;
  Mat mat_result = cvarrToMat(image_result).clone();
  ImgMatOperator::Mat_Raw_Write_Color(mat_result, filename_write);
    string str = "SLIC: " + to_string(nr_superpixels) + ", " + to_string(nc);
  //imshow(str, mat_result);
  //waitKey(0);
}


/*
 * @para
 * filename read : raw image data
 * filename_write: raw image write to
 * nr_superpixels: number of superpixels
 * nc: weight-factors
 */
void Run_MeanShift(string filename_read, string filename_write, int hs, double hc, int height, int width, int byte) {
    cout << "mean shift" << endl;
    Mat mat_original = ImgMatOperator::Mat_Raw_Read(filename_read, height, width, byte);
    IplImage image_original = mat_original;
    IplImage *img = cvCloneImage(&image_original);

    // Mean shift
    int **ilabels = new int *[img->height];
    for (int i = 0; i < img->height; i++)
        ilabels[i] = new int[img->width];
    int regionCount = MeanShift(img, ilabels, hs, hc);
    vector<int> color(regionCount);
    CvRNG rng = cvRNG(cvGetTickCount());
    for (int i = 0; i < regionCount; i++) {
        color[i] = cvRandInt(&rng);
    }

    // Draw random color
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int cl = ilabels[i][j];
            ((uchar *) (img->imageData + i * img->widthStep))[j * img->nChannels + 0] = (color[cl]) & 255;
            ((uchar *) (img->imageData + i * img->widthStep))[j * img->nChannels + 1] = (color[cl] >> 8) & 255;
            ((uchar *) (img->imageData + i * img->widthStep))[j * img->nChannels + 2] = (color[cl] >> 16) & 255;
        }
    }

    // Write to the file and show the image
    IplImage *image_result = img;
    Mat mat_result = cvarrToMat(image_result).clone();
    ImgMatOperator::Mat_Raw_Write_Color(mat_result, filename_write);
    string str = "MS: " + filename_read + to_string(hs) + ", " + to_string(hc);
    imshow(str, mat_result);
    waitKey(0);
    cvDestroyWindow("MeanShift");
    cvReleaseImage(&img);
}


void p3a2() {
  string filename = FOLDER + LABLE2 + ".raw";
    int height = 481 ;
    int width = 321;
    
    // test parameter: superpixel
    Run_Slic(filename, "Rhinos_100_10.raw", 100, 10, width, height, 3);
    Run_Slic(filename, "Rhinos_200_10.raw", 200, 10, width, height, 3);
    Run_Slic(filename, "Rhinos_300_10.raw", 300, 10, width, height, 3);
    Run_Slic(filename, "Rhinos_400_10.raw", 400, 10, width, height, 3);
    Run_Slic(filename, "Rhinos_100_50.raw", 100, 50, width, height, 3);
    Run_Slic(filename, "Rhinos_200_50.raw", 200, 50, width, height, 3);
    Run_Slic(filename, "Rhinos_300_50.raw", 300, 50, width, height, 3);
    Run_Slic(filename, "Rhinos_400_50.raw", 400, 50, width, height, 3);
    Run_Slic(filename, "Rhinos_100_100.raw", 100, 100, width, height, 3);
    Run_Slic(filename, "Rhinos_200_100.raw", 200, 100, width, height, 3);
    Run_Slic(filename, "Rhinos_300_100.raw", 300, 100, width, height, 3);
    Run_Slic(filename, "Rhinos_400_100.raw", 400, 100, width, height, 3);
    Run_Slic(filename, "Rhinos_100_200.raw", 100, 200, width, height, 3);
    Run_Slic(filename, "Rhinos_200_200.raw", 200, 200, width, height, 3);
    Run_Slic(filename, "Rhinos_300_200.raw", 300, 200, width, height, 3);
    Run_Slic(filename, "Rhinos_400_200.raw", 400, 200, width, height, 3);


    // test parameter: spatial radius
    /*Run_MeanShift("Rhinos_100_10.raw", "MS_Rhinos_100_10_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_200_10.raw", "MS_Rhinos_200_10_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_10.raw", "MS_Rhinos_300_10_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_400_10.raw", "MS_Rhinos_400_10_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_100_50.raw", "MS_Rhinos_100_50_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_400_50.raw", "MS_Rhinos_400_50_4_65.raw", 4, 6.5, width, height, 3);

    Run_MeanShift("Rhinos_100_10.raw", "MS_Rhinos_100_10_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_200_10.raw", "MS_Rhinos_200_10_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_10.raw", "MS_Rhinos_300_10_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_400_10.raw", "MS_Rhinos_400_10_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_100_50.raw", "MS_Rhinos_100_50_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_400_50.raw", "MS_Rhinos_400_50_10_65.raw", 10, 6.5, width, height, 3);

    Run_MeanShift("Rhinos_100_10.raw", "MS_Rhinos_100_10_14_65.raw", 14, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_200_10.raw", "MS_Rhinos_200_10_14_65.raw", 14, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_10.raw", "MS_Rhinos_300_10_14_65.raw", 14, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_400_10.raw", "MS_Rhinos_400_10_14_65.raw", 14, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_100_50.raw", "MS_Rhinos_100_50_14_65.raw", 14, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_14_65.raw", 14, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_14_65.raw", 14, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_400_50.raw", "MS_Rhinos_400_50_14_65.raw", 14, 6.5, width, height, 3);*/

    // test parameter: color radius
    /*Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_4_2.raw", 4, 2, width, height, 3);
    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_10_2.raw", 10, 2, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_4_2.raw", 4, 2, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_10_2.raw", 10, 2, width, height, 3);

    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_10_65.raw", 10, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_4_65.raw", 4, 6.5, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_10_65.raw", 10, 6.5, width, height, 3);

    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_4_12.raw", 4, 12, width, height, 3);
    Run_MeanShift("Rhinos_200_50.raw", "MS_Rhinos_200_50_10_12.raw", 10, 12, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_4_12.raw", 4, 12, width, height, 3);
    Run_MeanShift("Rhinos_300_50.raw", "MS_Rhinos_300_50_10_12.raw", 10, 12, width, height, 3);*/

}

void p3a1() {
    string filename = FOLDER + LABLE1 + ".raw";

    // test parameter: superpixel
    Run_Slic(filename, "Man_100_10.raw", 100, 10, 481, 321, 3);
    Run_Slic(filename, "Man_200_10.raw", 200, 10, 481, 321, 3);
    Run_Slic(filename, "Man_300_10.raw", 300, 10, 481, 321, 3);
    Run_Slic(filename, "Man_400_10.raw", 400, 10, 481, 321, 3);
    Run_Slic(filename, "Man_100_50.raw", 100, 50, 481, 321, 3);
    Run_Slic(filename, "Man_200_50.raw", 200, 50, 481, 321, 3);
    Run_Slic(filename, "Man_300_50.raw", 300, 50, 481, 321, 3);
    Run_Slic(filename, "Man_400_50.raw", 400, 50, 481, 321, 3);
    Run_Slic(filename, "Man_100_100.raw", 100, 100, 481, 321, 3);
    Run_Slic(filename, "Man_200_100.raw", 200, 100, 481, 321, 3);
    Run_Slic(filename, "Man_300_100.raw", 300, 100, 481, 321, 3);
    Run_Slic(filename, "Man_400_100.raw", 400, 100, 481, 321, 3);
    Run_Slic(filename, "Man_100_200.raw", 100, 200, 481, 321, 3);
    Run_Slic(filename, "Man_200_200.raw", 200, 200, 481, 321, 3);
    Run_Slic(filename, "Man_300_200.raw", 300, 200, 481, 321, 3);
    Run_Slic(filename, "Man_400_200.raw", 400, 200, 481, 321, 3);


    // test parameter: spatial radius
    /*Run_MeanShift("Man_100_10.raw", "MS_Man_100_10_4_65.raw", 4, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_10.raw", "MS_Man_200_10_4_65.raw", 4, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_10.raw", "MS_Man_300_10_4_65.raw", 4, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_10.raw", "MS_Man_400_10_4_65.raw", 4, 6.5, 481, 321, 3);
    Run_MeanShift("Man_100_50.raw", "MS_Man_100_50_4_65.raw", 4, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_50.raw", "MS_Man_200_50_4_65.raw", 4, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_50.raw", "MS_Man_300_50_4_65.raw", 4, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_50.raw", "MS_Man_400_50_4_65.raw", 4, 6.5, 481, 321, 3);*/

    /*Run_MeanShift("Man_100_10.raw", "MS_Man_100_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_10.raw", "MS_Man_200_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_10.raw", "MS_Man_300_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_10.raw", "MS_Man_400_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_100_50.raw", "MS_Man_100_50_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_50.raw", "MS_Man_200_50_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_50.raw", "MS_Man_300_50_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_50.raw", "MS_Man_400_50_10_65.raw", 10, 6.5, 481, 321, 3);*/

    /*Run_MeanShift("Man_100_10.raw", "MS_Man_100_10_14_65.raw", 14, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_10.raw", "MS_Man_200_10_14_65.raw", 14, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_10.raw", "MS_Man_300_10_14_65.raw", 14, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_10.raw", "MS_Man_400_10_14_65.raw", 14, 6.5, 481, 321, 3);
    Run_MeanShift("Man_100_50.raw", "MS_Man_100_50_14_65.raw", 14, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_50.raw", "MS_Man_200_50_14_65.raw", 14, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_50.raw", "MS_Man_300_50_14_65.raw", 14, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_50.raw", "MS_Man_400_50_14_65.raw", 14, 6.5, 481, 321, 3);*/

    // test parameter: color radius
    /*Run_MeanShift("Man_100_10.raw", "MS_Man_100_10_10_2.raw", 10, 2, 481, 321, 3);
    Run_MeanShift("Man_200_10.raw", "MS_Man_200_10_10_2.raw", 10, 2, 481, 321, 3);
    Run_MeanShift("Man_300_10.raw", "MS_Man_300_10_10_2.raw", 10, 2, 481, 321, 3);
    Run_MeanShift("Man_400_10.raw", "MS_Man_400_10_10_2.raw", 10, 2, 481, 321, 3);
    Run_MeanShift("Man_100_50.raw", "MS_Man_100_50_10_2.raw", 10, 2, 481, 321, 3);
    Run_MeanShift("Man_200_50.raw", "MS_Man_200_50_10_2.raw", 10, 2, 481, 321, 3);
    Run_MeanShift("Man_300_50.raw", "MS_Man_300_50_10_2.raw", 10, 2, 481, 321, 3);
    Run_MeanShift("Man_400_50.raw", "MS_Man_400_50_10_2.raw", 10, 2, 481, 321, 3);*/

    /*Run_MeanShift("Man_100_10.raw", "MS_Man_100_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_10.raw", "MS_Man_200_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_10.raw", "MS_Man_300_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_10.raw", "MS_Man_400_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_100_50.raw", "MS_Man_100_50_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_50.raw", "MS_Man_200_50_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_300_50.raw", "MS_Man_300_50_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_400_50.raw", "MS_Man_400_50_10_65.raw", 10, 6.5, 481, 321, 3);*/

    /*Run_MeanShift("Man_100_10.raw", "MS_Man_100_10_10_12.raw", 10, 12, 481, 321, 3);
    Run_MeanShift("Man_200_10.raw", "MS_Man_200_10_10_12.raw", 10, 12, 481, 321, 3);
    Run_MeanShift("Man_300_10.raw", "MS_Man_300_10_10_12.raw", 10, 12, 481, 321, 3);
    Run_MeanShift("Man_400_10.raw", "MS_Man_400_10_10_12.raw", 10, 12, 481, 321, 3);
    Run_MeanShift("Man_100_50.raw", "MS_Man_100_50_10_12.raw", 10, 12, 481, 321, 3);
    Run_MeanShift("Man_200_50.raw", "MS_Man_200_50_10_12.raw", 10, 12, 481, 321, 3);
    Run_MeanShift("Man_300_50.raw", "MS_Man_300_50_10_12.raw", 10, 12, 481, 321, 3);
    Run_MeanShift("Man_400_50.raw", "MS_Man_400_50_10_12.raw", 10, 12, 481, 321, 3);*/

}


int main(int argc, char *argv[]) {
    p3a1();
    p3a2();

  return 0;
}

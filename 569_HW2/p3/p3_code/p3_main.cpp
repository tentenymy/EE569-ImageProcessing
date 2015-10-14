/*
 * test_slic.cpp.
 *
 * Written by: Pascal Mettes.
 *
 * This file creates an over-segmentation of a provided image based on the SLIC
 * superpixel algorithm, as implemented in slic.h and slic.cpp.
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <float.h>
#include <opencv2/gpu/gpu.hpp>

#include "hw2_helper.h"
#include "slic.h"
using namespace std;

const string FOLDER = "p3_image/";

const string LABLE1 = "Man";
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
  //int nr_superpixels = 400;
  //int nc = 40;
  double step = sqrt((w * h) / (double) nr_superpixels);

  /* Perform the SLIC superpixel algorithm. */
  Slic slic;
  slic.generate_superpixels(lab_image, step, nc);
  slic.create_connectivity(lab_image);
  //slic.display_contours(&image_original, CV_RGB(255,0,0));
  slic.colour_with_cluster_means(&image_original);

  // Write to the file and show the image
  IplImage *image_result = &image_original;
  Mat mat_result = cvarrToMat(image_result).clone();
  ImgMatOperator::Mat_Raw_Write_Color(mat_result, filename_write);
  imshow("reslut", mat_result);
  waitKey(0);
}

void MeanShift() {
    unsigned long AAtime=0;

    //image load
    Mat img = imread("image2.jpg");
    Mat outImg, outimg2;

    //cpu version meanshift
    //gpu version meanshift
    gpu::GpuMat pimgGpu, imgGpu, outImgGpu;
    AAtime = getTickCount();
    pimgGpu.upload(img);
    //gpu meanshift only support 8uc4 type.
    gpu::cvtColor(pimgGpu, imgGpu, CV_BGR2BGRA);
    gpu::meanShiftFiltering(imgGpu, outImgGpu, 30, 30);
    outImgGpu.download(outimg2);
    ProccTimePrint(AAtime , "gpu");

    //show image
    imshow("origin", img);
    imshow("MeanShift Filter cpu", outImg);
    imshow("MeanShift Filter gpu", outimg2);
    waitKey(0);
}


void p3a() {
  string filename = FOLDER + LABLE1 + ".raw";
  /*Run_Slic(filename, "Man_50_40.raw", 50, 40, 481, 321, 3);
  Run_Slic(filename, "Man_100_40.raw", 100, 40, 481, 321, 3);
  Run_Slic(filename, "Man_200_40.raw", 200, 40, 481, 321, 3);
  Run_Slic(filename, "Man_300_40.raw", 300, 40, 481, 321, 3);
  Run_Slic(filename, "Man_400_40.raw", 400, 40, 481, 321, 3);
  Run_Slic(filename, "Man_500_40.raw", 500, 40, 481, 321, 3);
  Run_Slic(filename, "Man_600_40.raw", 600, 40, 481, 321, 3);*/
  /*Run_Slic(filename, "Man_300_10.raw", 300, 10, 481, 321, 3);
  Run_Slic(filename, "Man_300_20.raw", 300, 20, 481, 321, 3);
  Run_Slic(filename, "Man_300_30.raw", 300, 30, 481, 321, 3);
  Run_Slic(filename, "Man_300_40.raw", 300, 40, 481, 321, 3);
  Run_Slic(filename, "Man_300_50.raw", 300, 50, 481, 321, 3);
  Run_Slic(filename, "Man_300_80.raw", 300, 80, 481, 321, 3);
  Run_Slic(filename, "Man_300_100.raw", 300, 100, 481, 321, 3);*/
}


int main(int argc, char *argv[]) {
  p3a();

  return 0;
}

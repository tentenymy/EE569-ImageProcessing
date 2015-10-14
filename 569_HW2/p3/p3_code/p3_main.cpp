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
  //slic.display_contours(&image_original, CV_RGB(255,0,0));
  slic.colour_with_cluster_means(&image_original);

  // Write to the file and show the image
  IplImage *image_result = &image_original;
  Mat mat_result = cvarrToMat(image_result).clone();
  ImgMatOperator::Mat_Raw_Write_Color(mat_result, filename_write);
    string str = "SLIC: " + to_string(nr_superpixels) + ", " + to_string(nc);
  imshow(str, mat_result);
  waitKey(0);
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
    cout << "mean shift" << endl;
    int **ilabels = new int *[img->height];
    for (int i = 0; i < img->height; i++)
        ilabels[i] = new int[img->width];
    int regionCount = MeanShift(img, ilabels, hs, hc);

    cout << "mean shift" << endl;
    vector<int> color(regionCount);
    CvRNG rng = cvRNG(cvGetTickCount());

    cout << "COUNT:" << regionCount << endl;
    for (int i = 0; i < regionCount; i++) {
        color[i] = cvRandInt(&rng);
    }

    // Draw random color
    cout << "mean shift" << endl;
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int cl = ilabels[i][j];
            ((uchar *) (img->imageData + i * img->widthStep))[j * img->nChannels + 0] = (color[cl]) & 255;
            ((uchar *) (img->imageData + i * img->widthStep))[j * img->nChannels + 1] = (color[cl] >> 8) & 255;
            ((uchar *) (img->imageData + i * img->widthStep))[j * img->nChannels + 2] = (color[cl] >> 16) & 255;
        }
    }

    cout << "mean shift" << endl;
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


void p3a() {
  string filename = FOLDER + LABLE1 + ".raw";
    //Run_Slic(filename, "Man_50_10.raw", 50, 10, 481, 321, 3);
    //Run_Slic(filename, "Man_100_10.raw", 100, 10, 481, 321, 3);
    //Run_Slic(filename, "Man_200_10.raw", 200, 10, 481, 321, 3);

    //Run_Slic(filename, "Man_50_30.raw", 50, 30, 481, 321, 3);
    //Run_Slic(filename, "Man_100_30.raw", 100, 30, 481, 321, 3);
    //Run_Slic(filename, "Man_200_30.raw", 200, 30, 481, 321, 3);

    //Run_Slic(filename, "Man_50_50.raw", 50, 25, 481, 321, 3);
    //Run_Slic(filename, "Man_100_50.raw", 100, 50, 481, 321, 3);
    //Run_Slic(filename, "Man_200_50.raw", 200, 50, 481, 321, 3);
    //Run_Slic(filename, "Man_200_100.raw", 200, 100, 481, 321, 3);
    //Run_Slic(filename, "Man_200_150.raw", 200, 150, 481, 321, 3);
    //Run_Slic(filename, "Man_300_150.raw", 300, 150, 481, 321, 3);
    //Run_Slic(filename, "Man_300_30.raw", 300, 30, 481, 321, 3);
    //Run_Slic(filename, "Man_400_40.raw", 400, 40, 481, 321, 3);
    //Run_Slic(filename, "Man_500_40.raw", 500, 40, 481, 321, 3);
    //Run_Slic(filename, "Man_600_40.raw", 600, 40, 481, 321, 3);
    //Run_Slic(filename, "Man_300_10.raw", 300, 10, 481, 321, 3);
    //Run_Slic(filename, "Man_300_20.raw", 300, 20, 481, 321, 3);
    //Run_Slic(filename, "Man_300_30.raw", 300, 30, 481, 321, 3);
    //Run_Slic(filename, "Man_300_40.raw", 300, 40, 481, 321, 3);
    //Run_Slic(filename, "Man_300_50.raw", 300, 50, 481, 321, 3);
    //Run_Slic(filename, "Man_300_80.raw", 300, 80, 481, 321, 3);
    //Run_Slic(filename, "Man_300_100.raw", 300, 100, 481, 321, 3);*/


    Run_MeanShift("Man_100_10.raw", "MS_MAN_100_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_10.raw", "MS_MAN_200_10_10_65.raw", 10, 6.5, 481, 321, 3);
    Run_MeanShift("Man_200_50.raw", "MS_MAN_200_50_10_65.raw", 10, 6.5, 481, 321, 3);
}


int main(int argc, char *argv[]) {
  //p3a();

  return 0;
}

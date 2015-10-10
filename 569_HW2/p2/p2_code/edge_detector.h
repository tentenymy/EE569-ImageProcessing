/* EE569 Homework Assignment #2
 * Date: October 11, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * email: meiyiyan@usc.edu
 * Problem2. Edge Detection
 *
 *
 * Main function:
 * p2_main.cpp
 *
 * Class EdgeDetector:
 * EdgeDetector.h
 * EdgeDetector.cpp
 *
 * Class ImgMatOperator (General class for whole HW2)
 * hw2_helper.h
 * hw2_helper.cpp
 *
 * p2c: matlab code
 * prob2c.m
 * detector_SE.m
 */

#ifndef P2_EDGE_DETECTOR_H
#define P2_EDGE_DETECTOR_H

#include "hw2_helper.h"
using namespace std;
using namespace cv;

class EdgeDetector {
private:
    int height;
    int width;
    int byteperpixel;
    Mat mat_gray;
    Mat mat_original;

public:
    // construction;
    EdgeDetector(string filename, int new_height, int new_width, int new_byteperpixel);
    //EdgeDetector(Mat mat, int new_height, int new_width, int new_byteperpixel);
    //EdgeDetector(Img *pt_img, int new_height, int new_width, int new_byteperpixel);

    void ConvertRGB();
    void Detector_Sobel(double threshold);
    void Detector_Canny(double coef_A, double coef_B);

    static void Evaluation(string filename_measure, vector<string> filename_gt, int, int, int);

    // Helper function
    // mode: 1 normalization; 0 no normalization;
    void Print_Raw(Mat mat, string filename, int mode, int byte);
    //void Print_Mat(Mat mat, string name);
    //void Print_Detail();
};
#endif //P2_EDGE_DETECTOR_H

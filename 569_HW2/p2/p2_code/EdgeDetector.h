//
// Created by Maggie on 10/10/15.
//

#ifndef P2_EDGEDETECTOR_H
#define P2_EDGEDETECTOR_H

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



#endif //P2_EDGEDETECTOR_H

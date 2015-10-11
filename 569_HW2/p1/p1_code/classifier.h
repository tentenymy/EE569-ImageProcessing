/* EE569 Homework Assignment #2
 * Date: October 11, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * email: meiyiyan@usc.edu
 * Problem1. Texture Analysis and Classification
 *
 * Main function:
 * p1_main.cpp
 *
 * Class EdgeDetector:
 * classifier.h
 * classifier.cpp
 *
 * Class ImgMatOperator (General class for whole HW2)
 * hw2_helper.h
 * hw2_helper.cpp
 */

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "hw2_helper.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>

using namespace cv;
using namespace cv::ml;


// Declaration of default parameter
// Image
const int HEIGHT = 128;
const int WIDTH = 128;
const int BYTEPERPIXEL = 1;
// Law's filter
const int NUM_DATA = 25;
// Mode
const int MODE_MM = 0;
const int MODE_PCA = 1;
const int MODE_LDA = 2;


class Classifier {
private:
    // 25 5 * 5 Law's Filter
    int filter_bank[25][25];

    // input training and testing data
    vector <string> list_filename_train;
    vector <string> list_label_train;
    vector <string> list_filename_test;
    vector <string> list_label_test;
    int num_test;
    int num_train;
    int num_label;
    vector <string> list_label_name;
    vector <int> list_num_per_label;

    // extract feature
    vector <Mat> mat_feature;
    vector <Mat> mat_feature_average;
    Mat mat_feature_pca_lda;
    vector <Mat> mat_feature_average_pca;
    vector <Mat> mat_feature_pca;
    vector <Mat> mat_feature_average_lda;
    vector <Mat> mat_feature_lda;

    // error rate
    int count_error_train;
    int count_error_test;

    // functions
    double *Extract_Feature (Img*, int, int, int);
    void Decrease_Dimension_PCA(int);
    void Decrease_Dimension_LDA(int);
    int Get_Minimum_Mean_Distance (int, string, int);

public:
    Classifier(vector<string>, vector<string>, vector<string>, vector<string>);
    void Set_Feature();
    void Classify_MM(int mode, int num);
    void Classify_SVM(int mode, int num);

    void Print_Error_Rate();
    void Print_Stat(int);
    void Print_Label();
};

#endif //P1_CLASSIFER_H

//
// Created by Maggie on 10/6/15.
//

#ifndef P1_CLASSIFER_H
#define P1_CLASSIFER_H

#include "hw2_helper.h"


// Image
const int HEIGHT = 128;
const int WIDTH = 128;
const int BYTEPERPIXEL = 1;

// Filter
const int NUM_DATA = 25;

// MODE
const int MODE_MM = 0;
const int MODE_PCA = 1;
const int MODE_LDA = 2;



// Data

const string LIST_LABEL[3] = {"grass", "straw", "unknown"};
const int LIST_NUM_LABEL[3] = {36, 36, 24};
const int NUM_LABEL = 2;


class Classifier {

private:
    int filter_bank[25][25]; // 25 5 * 5 Law's Filter

    vector <string> list_filename_train;
    vector <string> list_label_train;
    vector <string> list_filename_test;
    vector <string> list_label_test;

    int num_test;
    int num_train;

    int num_label;
    vector <string> list_label_name;
    vector <int> list_num_per_label;



    vector <Mat> mat_feature;
    Mat mat_feature_pca_lda;
    vector <Mat> mat_feature_average;

    int count_error_train;
    int count_error_test;

    vector <Mat> mat_feature_average_pca;
    vector <Mat> mat_feature_average_lda;

    vector <Mat> mat_feature_pca;
    vector <Mat> mat_feature_lda;
    double *Extract_Feature (Img *pt_img, int height, int width, int byteperpixel);
    int Get_Minimum_Mean_Distance (int no_feature, string label, int mode);
    void Decrease_Dimension_PCA(int num);
    void Decrease_Dimension_LDA(int num);

public:
    Classifier(vector <string>, vector <string>, vector <string>, vector <string>);

    void Set_Feature();
    void Classify_MM(int mode);

    void Classifier_PCA(int num);
    void Classifier_LDA(int num);
    void Classifier_SVM(int mode, int num);

    void Print_Error_Rate();
    void Print_Label();
    void Print_Stat(int);
};

#endif //P1_CLASSIFER_H

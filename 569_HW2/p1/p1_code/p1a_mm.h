//
// Created by Maggie on 9/30/15.
//

#ifndef HW2_P1A_H
#define P1A_MM_H
#endif //HW2_P1A_H

using namespace cv;
using namespace std;

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

    int mode = 0;
    int mode_num = 0;

    vector <Mat> mat_feature;
    Mat mat_feature_average[NUM_LABEL];
    Mat mat_feature_pca_lda;
    Mat mat_feature_average_pca[NUM_LABEL];
    Mat mat_feature_average_lda[NUM_LABEL];

    vector <Mat> mat_feature_pca;
    vector <Mat> mat_feature_lda;





    void Set_Filter();

    int *Get_Window(int i, int j, int height, int width, int half_window);

    int Get_Minimum_Mean_Distance (int no_feature, string label);
    int Get_Minimum_Mean_Distance_PCA (int no_feature, string label);
    int Get_Minimum_Mean_Distance_LDA (int no_feature, string label);

public:
    Classifier();
    double *Extract_Feature (Img *pt_img, int height, int width, int byteperpixel);
    void Set_Feature_Known ();
    void Set_Feature_Unknown();

    void Classifier_MM();
    void Classifier_PCA(int num);
    void Classifier_LDA(int num);
};
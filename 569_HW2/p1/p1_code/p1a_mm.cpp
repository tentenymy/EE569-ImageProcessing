#include "hw2_helper.h"
#include "p1a_mm.h"

using namespace cv;
using namespace std;



// Get file and label
const string LABEL[5] = {"grass", "straw", "sand", "leather", "unknown"};
const int GRASS = 0;
const int STRAW = 1;
const int SAND = 2;
const int LEATHER = 3;
const int UNKOWN = 4;

string list_filename_train[72];
string list_filename_test[24];
string list_label_train[72];
string list_label_test[24];

const string FOLDER1 = "p1_image/p1_image_a/";
const string FOLDER2 = "p1_image/p1_image_a/";

string Get_Filename (string label, int num);
void Get_List_Filename ();

// Local Function
string Get_Filename (string label, int num) {
    string str_num;
    num++;
    if (num < 10)
        str_num = "0" + to_string(num);
    else
        str_num = to_string(num);
    return label + "_" + str_num + ".raw";
}
void Get_List_Filename () {
    // Get filename and label list for training set
    for (int i = 0; i < 72; i++) {
        if (i < 36) {
            list_label_train[i] = LABEL[0];
            list_filename_train[i] = FOLDER1 + Get_Filename(LABEL[0], i);
        }
        else {
            list_label_train[i] = LABEL[1];
            list_filename_train[i] = FOLDER1 + Get_Filename(LABEL[1], i - 36);
        }
    }
    // Get filename and label list for testing set
    string temp_list_label_test[24] =
            {"straw", "straw", "grass", "grass", "grass", "straw",
             "grass", "straw", "straw", "grass", "grass", "grass",
             "straw", "grass", "straw" , "straw", "straw", "straw",
             "grass", "grass", "grass", "straw", "straw", "grass"};
    for (int i = 0; i < 24; i++)
    {
        list_filename_test[i] = FOLDER1 + Get_Filename(LIST_LABEL[2], i);
        list_label_test[i] = temp_list_label_test[i];
    }
}


// Constructor
Classifier::Classifier() {
    // Create Laws' Filter
    int coef_laws_filter[5][5] = {{1, 4, 6, 4, 1}, {-1, -2, 0, 2, 1}, {-1, 0, 2, 0, -1},
                                  {-1, 2, 0, -2, 1}, {1, -4, 6, -4, 1}};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int m = 0; m < 5; m++) {
                for (int n = 0; n < 5; n++) {
                    filter_bank[i * 5 + j][m * 5 + n] = coef_laws_filter[i][m] * coef_laws_filter[j][n];
                }
            }
        }
    }
}

// Private Function
int *Classifier::Get_Window(int i, int j, int height, int width, int half_window) {
    // top, bottom, left, right
    int window[4] = {i - 2, i + 2, j - 2, j + 2};
    if (window[0] < 0)
        window[0] = 0;
    if (window[2] < 0)
        window[2] = 0;
    if (window[1] > height - 1)
        window[1] = height - 1;
    if (window[3] > width - 1)
        window[3] = width - 1;
    int *res = window;
    return res;
}
int Classifier::Get_Minimum_Mean_Distance (int num, string label) {
    Mat temp_covar, temp_covar_invert;
    double distance[NUM_LABEL];
    double min_distance = 100;
    string min_label = "";
    for (int i = 0; i < NUM_LABEL; i++) {
        calcCovarMatrix(mat_feature[num], temp_covar, mat_feature_average[i],
                        CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_USE_AVG);
        invert(temp_covar, temp_covar_invert, DECOMP_SVD);
        distance[i] = Mahalanobis(mat_feature[num], mat_feature_average[i], temp_covar);
        if (min_distance > distance[i]) {
            min_distance = distance[i];
            min_label = LIST_LABEL[i];
        }
    }
    if (min_label != label) {
        cout << label << " " << num << ": " << min_label << " (*)" << endl;
        return 1;
    }
    else  {
        cout << label << " " << num << ": " << min_label << endl;
        return 0;
    }
}
int Classifier::Get_Minimum_Mean_Distance_PCA (int num, string label) {
    Mat temp_covar, temp_covar_invert;
    double distance[NUM_LABEL];
    double min_distance = 100;
    string min_label = "";
    for (int i = 0; i < NUM_LABEL; i++) {
        calcCovarMatrix(mat_feature_pca[num], temp_covar, mat_feature_average_pca[i],
                        CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_USE_AVG);
        invert(temp_covar, temp_covar_invert, DECOMP_SVD);
        distance[i] = Mahalanobis(mat_feature_pca[num], mat_feature_average_pca[i], temp_covar);
        if (min_distance > distance[i]) {
            min_distance = distance[i];
            min_label = LIST_LABEL[i];
        }
    }
    if (min_label != label) {
        cout << label << " " << num << ": " << min_label << " (*)" << endl;
        return 1;
    }
    else  {
        cout << label << " " << num << ": " << min_label << endl;
        return 0;
    }
}
int Classifier::Get_Minimum_Mean_Distance_LDA (int num, string label) {
    Mat temp_covar, temp_covar_invert;
    double distance[NUM_LABEL];
    double min_distance = 100;
    string min_label = "";
    for (int i = 0; i < NUM_LABEL; i++) {
        calcCovarMatrix(mat_feature_lda[num], temp_covar, mat_feature_average_lda[i],
                        CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_USE_AVG);
        invert(temp_covar, temp_covar_invert, DECOMP_SVD);
        distance[i] = Mahalanobis(mat_feature_lda[num], mat_feature_average_lda[i], temp_covar);
        if (min_distance > distance[i]) {
            min_distance = distance[i];
            min_label = LIST_LABEL[i];
        }
    }
    if (min_label != label) {
        cout << label << " " << num << ": " << min_label << " (*)" << endl;
        return 1;
    }
    else  {
        cout << label << " " << num << ": " << min_label << endl;
        return 0;
    }
}


// Public Function
double *Classifier::Extract_Feature (Img *pt_img, int height, int width, int byteperpixel) {
    //cout << "Feature Extract: " << endl;
    // 1. Set original image
    int img[128 * 128];
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 128; j++) {
            img[i * 128 + j] = (int)*pt_img++;
        }
    }
    // 2. Subtract local mean from original image
    int img_mean[128 * 128];
    int img_subtract[128 * 128];
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 128; j++) {
            int temp_num = 0;
            double temp_total = 0;
            int *window = Get_Window(i , j, 128, 128, 2);
            for (int m = window[0]; m <= window[1]; m++) {
                for (int n = window[2]; n <= window[3]; n++) {
                    temp_total += img[m * 128 + n];
                    temp_num++;
                }
            }
            img_mean[i * 128 + j] = round(temp_total/temp_num);
            img_subtract[i * 128 + j] = img[i * 128 + j] - img_mean[i * 128 + j];
        }
    }
    // 3. Apply Laws' filter to subtracted image and get the average of energy of each pixel
    double img_energy[25][128 * 128];
    double pixel_num = 128 * 128;
    double pixel_total[25];
    for (int k = 0; k < 25; k++) {
        pixel_total[k] = 0;
        for (int i = 0; i < 128; i++) {
            for (int j = 0; j < 128; j++) {
                int temp_num = 0;
                double temp_total = 0;
                int *window = Get_Window(i , j, 128, 128, 2);
                for (int m = window[0]; m <= window[1]; m++) {
                    for (int n = window[2]; n <= window[3]; n++) {
                        temp_total += img_subtract[m * 128 + n] * filter_bank[k][5 * (2 + m - i) + (2 + n - j)];
                        temp_num ++;
                    }
                }
                img_energy[k][128 * i + j] = (temp_total / temp_num) * (temp_total / temp_num);
                pixel_total[k] += img_energy[k][128 * i + j];
            }
        }
        pixel_total[k] = pixel_total[k] / pixel_num;
    }
    // 4. Normalization X' = (X - Xmin) / (Xmax - Xmin)
    double pixel_min = pixel_total[0];
    double pixel_max = pixel_total[0];
    for (int i = 1; i < 25; i++) {
        if (pixel_total[i] < pixel_min) {
            pixel_min = pixel_total[i];
        }
        if (pixel_total[i] > pixel_max) {
            pixel_max = pixel_total[i];
        }
    }
    for (int i = 0; i < 25; i++) {
        pixel_total[i] = (pixel_total[i] - pixel_min) / (pixel_max - pixel_min);
    }
    double *res = pixel_total;
    return res;
}
void Classifier::Set_Feature_Known () {
    cout << "Set_Feature_Known" << endl;
    // Read all labeled image and get mean
    ImgMatOperator img_op;
    Img *temp_img;
    double *temp_feature;
    double feature_average[NUM_LABEL][NUM_DATA] = {};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 36; j++) {
            temp_img = img_op.Img_Raw_Read(list_filename_train[i * 36 + j], HEIGHT, WIDTH, BYTEPERPIXEL);
            temp_feature = Extract_Feature(temp_img, HEIGHT, WIDTH, BYTEPERPIXEL);
            for (int j = 0; j < NUM_DATA; j++) {
                feature_average[i][j] += temp_feature[j] /36;
            }
            Mat temp_mat = Mat(1, NUM_DATA, CV_64F, temp_feature).clone();
            mat_feature.push_back(temp_mat);
        }
    }


    // Save in Mat
    for (int i = 0; i < NUM_LABEL; i++) {
        mat_feature_average[i] = Mat(1, NUM_DATA, CV_64F, feature_average[i]).clone();
    }
    // Set a big matrix for later use (PCA, LDA)
    mat_feature_pca_lda = mat_feature[0];
    for (int i = 1; i < 72; i++) {
        vconcat(mat_feature_pca_lda, mat_feature[i], mat_feature_pca_lda);
    }
}
void Classifier::Set_Feature_Unknown() {
    cout << "Set_Feature_Unknown" << endl;
    // Read all labeled image and get mean
    ImgMatOperator img_op;
    Img *temp_img;
    double *temp_feature;
    for (int i = 0; i < 24; i++) {
        temp_img = img_op.Img_Raw_Read(list_filename_test[i], HEIGHT, WIDTH, BYTEPERPIXEL);
        temp_feature = Extract_Feature(temp_img, HEIGHT, WIDTH, BYTEPERPIXEL);
        Mat temp_mat = Mat(1, NUM_DATA, CV_64F, temp_feature).clone();
        mat_feature.push_back(temp_mat);
    }
}
void Classifier::Classifier_MM() {
    cout << "Minimum Mean Distance" << endl;
    ImgMatOperator img_op;
    int count_error = 0;
    // Read Extracted Feature
    for (int k = 0; k < 36; k++) {
        count_error += Get_Minimum_Mean_Distance (k, LIST_LABEL[0]);
    }
    for (int k = 0; k < 36; k++) {
        count_error += Get_Minimum_Mean_Distance (k + 36, LIST_LABEL[1]);
    }
    for (int k = 0; k < 24; k++) {
        count_error += Get_Minimum_Mean_Distance (k + 72, list_label_test[k]);
    }
    double error_rate = 100.0 * count_error / (36.0 + 36.0 + 24.0);
    cout << "ERROR RATE: " << error_rate << endl;
}
void Classifier::Classifier_PCA(int num) {
    // Use PCA reduce the feature matrix
    cout << "Classifier_PCA" << endl;
    // Get 36 num-dimension feature matrix
    PCA pca = PCA (mat_feature_pca_lda, Mat(), CV_PCA_DATA_AS_ROW, num);
    Mat mat_projected = Mat(72, num, CV_64F);
    pca.project(mat_feature_pca_lda, mat_projected);
    for (int i = 0; i < 72; i++) {
        Mat mat_temp;
        mat_projected.row(i).copyTo(mat_temp);
        mat_feature_pca.push_back(mat_temp);
    }
    cout << mat_projected << endl << endl;
    // Get 24 unknown image 2-D
    for (int i = 0; i < 24; i++) {
        mat_projected = pca.project(mat_feature[72 + i]);
        mat_feature_pca.push_back(mat_projected);
        cout << mat_projected << endl;
    }
    cout << endl;
    // Get 2 classes num-dismension mean matrix
    cout << "Average: " << endl;
    Mat mat_temp[2];
    for (int i = 0; i < 2; i++) {
        mat_temp[i] = mat_feature_pca[0];
        for (int j = 0; j < 36; j++) {
            vconcat(mat_temp[i], mat_feature_pca[i * 36 + j], mat_temp[i]);
        }
        reduce(mat_temp[i], mat_feature_average_pca[i], 0, CV_REDUCE_AVG);
        cout << mat_feature_average_pca[i] << endl << endl;
    }
    cout << "Classifier_PCA" << endl;
    // Use Minimum_Mean_Distance to get error rate
    ImgMatOperator img_op;
    int count_error = 0;
    for (int k = 0; k < 36; k++) {
        count_error += Get_Minimum_Mean_Distance_PCA (k, LIST_LABEL[0]);
    }
    for (int k = 0; k < 36; k++) {
        count_error += Get_Minimum_Mean_Distance_PCA (k + 36, LIST_LABEL[1]);
    }
    for (int k = 0; k < 24; k++) {
        count_error += Get_Minimum_Mean_Distance_PCA (k + 72, list_label_test[k]);
    }
    double error_rate = 100.0 * count_error / (36.0 + 36.0 + 24.0);
    cout << "ERROR RATE: " << error_rate << endl;
}
void Classifier::Classifier_LDA(int num) {
    // Use LDA reduce the feature matrix
    cout << "Classifier_LDA" << endl;
    Mat list_label_known;
    int list_label[72];
    for (int i = 0; i < NUM_LABEL; i++) {
        for (int j = 0; j < 36; j++) {
            list_label[i * 36 + j] = i;
        }
    }
    list_label_known = Mat(1, 72, CV_32S, list_label);


    // Get 36 num-dimension feature matrix
    LDA lda = LDA (mat_feature_pca_lda, list_label_known, num);
    Mat mat_projected = Mat(72, num, CV_64F);
    mat_projected = lda.project(mat_feature_pca_lda);
    for (int i = 0; i < 72; i++) {
        Mat mat_temp;
        mat_projected.row(i).copyTo(mat_temp);
        mat_feature_lda.push_back(mat_temp);
    }
    cout << mat_projected << endl << endl;
    // Get 24 unknown image 2-D
    for (int i = 0; i < 24; i++) {
        mat_projected = lda.project(mat_feature[72 + i]);
        mat_feature_lda.push_back(mat_projected);
        cout << mat_projected << endl;
    }
    cout << endl;
    // Get 2 classes num-dismension mean matrix
    cout << "Average: " << endl;
    Mat mat_temp[2];
    for (int i = 0; i < 2; i++) {
        mat_temp[i] = mat_feature_lda[0];
        for (int j = 0; j < 36; j++) {
            vconcat(mat_temp[i], mat_feature_lda[i * 36 + j], mat_temp[i]);
        }
        reduce(mat_temp[i], mat_feature_average_lda[i], 0, CV_REDUCE_AVG);
        cout << mat_feature_average_lda[i] << endl;
    }
    cout << "Classifier_LDA" << endl;
    // Use Minimum_Mean_Distance to get error rate
    ImgMatOperator img_op;
    int count_error = 0;
    for (int k = 0; k < 36; k++) {
        count_error += Get_Minimum_Mean_Distance_LDA (k, LIST_LABEL[0]);
    }
    cout << "Classifier_LDA" << endl;
    for (int k = 0; k < 36; k++) {
        count_error += Get_Minimum_Mean_Distance_LDA (k + 36, LIST_LABEL[1]);
    }
    cout << "Classifier_LDA" << endl;
    for (int k = 0; k < 24; k++) {
        count_error += Get_Minimum_Mean_Distance_LDA (k + 72, list_label_test[k]);
    }
    double error_rate = 100.0 * count_error / (36.0 + 36.0 + 24.0);
    cout << "ERROR RATE: " << error_rate << endl;
}





void Prob1a()
{
    cout << "Prob1a" << endl;
    Get_List_Filename();
    Classifier classifier = Classifier();
    classifier.Set_Feature_Known();
    classifier.Set_Feature_Unknown();

    //classifier.Classifier_MM();
    classifier.Classifier_PCA(1);
    //classifier.Classifier_PCA(2);
    //classifier.Classifier_LDA(1);
}

int main(int argc, char *argv[])
{
    cout << "Problem 1" << endl;
    Prob1a();

    return 0;
}


//
// Created by Maggie on 10/6/15.
//

#include "classifer.h"

Classifier::Classifier(vector <string> new_list_filename_train, vector <string> new_list_label_train,
        vector <string> new_list_filename_test, vector <string> new_list_label_test) {
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
    // Save testing and training files and labels
    list_filename_train = new_list_filename_train;
    list_filename_test = new_list_filename_test;
    list_label_train = new_list_label_train;
    list_label_test = new_list_label_test;
    num_train = list_filename_train.size();
    num_test = list_filename_test.size();
    // Calculate the number of different labels in the training data
    // Labels must be continuous
    string temp_label = list_label_train[0];
    int temp_num_per_label = 1;
    list_label_name.push_back(temp_label);
    for (int i = 1; i < num_train; i++) {
        if (list_label_train[i] != temp_label) {
            temp_label = list_label_train[i];
            list_label_name.push_back(temp_label);
            list_num_per_label.push_back(temp_num_per_label);
            temp_num_per_label = 0;
        }
        temp_num_per_label++;
    }
    list_num_per_label.push_back(temp_num_per_label);
    num_label = list_num_per_label.size();
}

double *Classifier::Extract_Feature (Img *pt_img, int height, int width, int byteperpixel) {
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
            int *window = ImgMatOperator::Get_Window(i , j, 128, 128, 2);
            for (int m = window[0]; m <= window[1]; m++) {
                for (int n = window[2]; n <= window[3]; n++) {
                    temp_total += img[m * 128 + n];
                    temp_num++;
                }
            }
            //img_mean[i * 128 + j] = round(temp_total/temp_num);
            img_subtract[i * 128 + j] = img[i * 128 + j] - round(temp_total/temp_num);
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
                int *window = ImgMatOperator::Get_Window(i , j, 128, 128, 2);
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
void Classifier::Set_Feature () {
    cout << "Set_Feature_Train: " << num_train << endl;
    // For training data
    int count_train = 0;
    for (int i = 0; i < num_label; i++) {
        double temp_feature_average[NUM_DATA] = {};
        for (int j = 0; j < list_num_per_label[i]; j++) {
            Img *temp_img = ImgMatOperator::Img_Raw_Read(list_filename_train[count_train++], HEIGHT, WIDTH, BYTEPERPIXEL);
            double *temp_feature = Extract_Feature(temp_img, HEIGHT, WIDTH, BYTEPERPIXEL);
            for (int j = 0; j < NUM_DATA; j++) {
                temp_feature_average[j] += temp_feature[j] /list_num_per_label[i];
            }
            Mat temp_mat = Mat(1, NUM_DATA, CV_64F, temp_feature).clone();
            mat_feature.push_back(temp_mat);
        }
        mat_feature_average.push_back(Mat(1, NUM_DATA, CV_64F, temp_feature_average).clone());
    }
    // Set a big matrix for later use (PCA, LDA)
    mat_feature_pca_lda = mat_feature[0];
    for (int i = 1; i < num_train; i++) {
        vconcat(mat_feature_pca_lda, mat_feature[i], mat_feature_pca_lda);
    }
    cout << "Set_Feature_Test: " << num_test << endl;
    // For testing data
    for (int i = 0; i < num_test; i++) {
        Img *temp_img = ImgMatOperator::Img_Raw_Read(list_filename_test[i], HEIGHT, WIDTH, BYTEPERPIXEL);
        double *temp_feature = Extract_Feature(temp_img, HEIGHT, WIDTH, BYTEPERPIXEL);
        Mat temp_mat = Mat(1, NUM_DATA, CV_64F, temp_feature).clone();
        mat_feature.push_back(temp_mat);
    }
}

int Classifier::Get_Minimum_Mean_Distance (int num, string label, int mode) {
    Mat temp_covar, temp_covar_invert;
    vector<double> distance;
    double min_distance = 100;
    string min_label = "";
    if (mode == MODE_MM) {
        for (int i = 0; i < num_label; i++) {
            calcCovarMatrix(mat_feature[num], temp_covar, mat_feature_average[i],
                            CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_USE_AVG);
            invert(temp_covar, temp_covar_invert, DECOMP_SVD);
            distance.push_back(Mahalanobis(mat_feature[num], mat_feature_average[i], temp_covar));
        }
    } else if (mode == MODE_PCA) {
        for (int i = 0; i < num_label; i++) {
            calcCovarMatrix(mat_feature_pca[num], temp_covar, mat_feature_average_pca[i],
                            CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_USE_AVG);
            invert(temp_covar, temp_covar_invert, DECOMP_SVD);
            distance.push_back(Mahalanobis(mat_feature_pca[num], mat_feature_average_pca[i], temp_covar));
        }
    } else {
        for (int i = 0; i < num_label; i++) {
            calcCovarMatrix(mat_feature_lda[num], temp_covar, mat_feature_average_lda[i],
                            CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_USE_AVG);
            invert(temp_covar, temp_covar_invert, DECOMP_SVD);
            distance.push_back(Mahalanobis(mat_feature_lda[num], mat_feature_average_lda[i], temp_covar));
        }
    }
    for (int i = 0; i < num_label; i++) {
        if (min_distance > distance[i]) {
            min_distance = distance[i];
            min_label = list_label_name[i];
        }
    }
    // Print result details
    if (num < num_train) {
        if (min_label != label) {
            //cout << label << " " << list_filename_train[num] << " " << num << ": " << min_label << " (*)" << endl;
            return 1;
        }
        else  {
            //cout << label << " " << list_filename_train[num] << " " << num << ": " << min_label << endl;
            return 0;
        }
    } else {
        if (min_label != label) {
            //cout << label << " " << list_filename_test[num - num_train] << " " << num << ": " << min_label << " (*)" << endl;
            return 1;
        }
        else  {
            //cout << label << " " << list_filename_test[num - num_train] << " " << num << ": " << min_label << endl;
            return 0;
        }
    }
}
void Classifier::Classify_MM(int mode, int num) {
    cout << endl << "Classify_MM  Mode: " << mode << " num: " << num << endl;
    if (mode == MODE_PCA && num > 0 && num < 25) {
        Decrease_Dimension_PCA(num);
    } else if (mode == MODE_LDA && num > 0 && num < 25){
        Decrease_Dimension_LDA(num);
    }
    count_error_train = 0;
    count_error_test = 0;
    int count = 0;
    // Read Extracted Feature for training data
    for (int i = 0; i < num_label; i++) {
        for (int k = 0; k < list_num_per_label[i]; k++) {
            count_error_train += Get_Minimum_Mean_Distance (count++, list_label_name[i], mode);
        }
    }
    // Read Extracted Feature for testing data
    for (int k = 0; k < num_test; k++) {
        count_error_test += Get_Minimum_Mean_Distance (count++, list_label_test[k], mode);
    }
}

void Classifier::Decrease_Dimension_PCA(int num) {
    cout << "Decrease_Dimension_PCA" << endl;
    // Get training data set (num-dimension)
    PCA pca = PCA (mat_feature_pca_lda, Mat(), CV_PCA_DATA_AS_ROW, num);
    Mat mat_projected = Mat(num_train, num, CV_64F).clone();
    pca.project(mat_feature_pca_lda, mat_projected);
    for (int i = 0; i < num_train; i++) {
        Mat mat_temp;
        mat_projected.row(i).copyTo(mat_temp);
        mat_feature_pca.push_back(mat_temp);
    }
    // Get testing data set (num-dimension)
    for (int i = 0; i < num_test; i++) {
        mat_projected = pca.project(mat_feature[num_train + i]);
        mat_feature_pca.push_back(mat_projected);
    }
    // Get 2 classes num-dismension mean matrix
    vector<Mat> mat_temp;
    int count = 0;
    for (int i = 0; i < num_label; i++) {
        Mat temp_average;
        mat_temp.push_back(mat_feature_pca[0]);
        for (int j = 0; j < list_num_per_label[i]; j++) {
            vconcat(mat_temp[i], mat_feature_pca[count++], mat_temp[i]);
        }
        reduce(mat_temp[i], temp_average, 0, CV_REDUCE_AVG);
        mat_feature_average_pca.push_back(temp_average);
    }
}
void Classifier::Decrease_Dimension_LDA(int num) {
    cout << "Decrease_Dimension_LDA" << endl;
    //Convert Label
    Mat list_label_known;
    const int NUM_TRAIN = num_train;
    const int NUM_LABEL = num_label;
    int list_label[NUM_TRAIN];
    int count = 0;
    for (int i = 0; i < num_label; i++) {
        for (int j = 0; j < list_num_per_label[i]; j++) {
            list_label[count++] = i;
        }
    }
    list_label_known = Mat(1, num_train, CV_32S, list_label).clone();

    // Get training data set (num-dimension)
    LDA lda = LDA (mat_feature_pca_lda, list_label_known, num);
    Mat mat_projected = Mat(num_train, num, CV_64F).clone();
    mat_projected = lda.project(mat_feature_pca_lda);
    for (int i = 0; i < num_train; i++) {
        Mat mat_temp;
        mat_projected.row(i).copyTo(mat_temp);
        mat_feature_lda.push_back(mat_temp);
    }

    // Get testing data set (num-dimension)
    for (int i = 0; i < num_test; i++) {
        mat_projected = lda.project(mat_feature[num_train + i]);
        mat_feature_lda.push_back(mat_projected);
    }

    // Get 2 classes num-dismension mean matrix
    vector<Mat> mat_temp;
    count = 0;
    for (int i = 0; i < num_label; i++) {
        Mat temp_average;
        mat_temp.push_back(mat_feature_lda[0]);
        for (int j = 0; j < list_num_per_label[i]; j++) {
            vconcat(mat_temp[i], mat_feature_lda[count++], mat_temp[i]);
        }
        reduce(mat_temp[i], temp_average, 0, CV_REDUCE_AVG);
        mat_feature_average_lda.push_back(temp_average);
    }
}

void Classifier::Classify_SVM(int mode, int num) {
    cout << endl << "Classify_SVM  Mode: " << mode << " num: " << num << endl;
    if (mode == MODE_PCA && num > 0 && num < 25) {
        Decrease_Dimension_PCA(num);
    }
    // SVM
    // Label
    const int NUM_TRAIN = num_train;
    int list_label[NUM_TRAIN];
    for (int i = 0; i < num_train; i++) {
        if (i < list_num_per_label[0])
            list_label[i] = 1;
        else
            list_label[i] = -1;
    }
    Mat labelsMat = Mat(num_train, 1, CV_32SC1, list_label).clone();
    // Scaling!!!!
    Mat mat_mean = Mat::zeros(1, num, CV_32F);
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < mat_feature_average_pca.size(); j++) {
            mat_mean.at<float>(0, i) += mat_feature_average_pca[j].at<double>(0, i) * list_num_per_label[j] / num_train;
        }
    }
    const int NUM = num;
    float scale[NUM];
    for (int i = 0; i < num; i++) {
        float total = 0;
        for (int j = 0; j < num_train; j++) {
            float value = mat_feature_pca[j].at<double>(0, i);
            total += (value - mat_mean.at<float>(0, i)) * (value - mat_mean.at<float>(0, i)) / (num_train - 1);
        }
        scale[i] = sqrt(total);
        cout << "scale: " << scale[1] << endl;
    }
    // Set Training data
    Mat mat_train = Mat(num_train, num, CV_32F).clone();
    for (int i = 0; i < num_train; i++) {
        for (int j = 0; j < num; j++) {
            float value = (float)mat_feature_pca[i].at<double>(0, j) / scale[j] ;
            mat_train.at<float>(i, j) = value;
        }
    }
    // SVM train
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(mat_train, ROW_SAMPLE, labelsMat);
    // SVM predict
    vector<float> res;
    Mat mat_test = Mat(1, num, CV_32F);
    for (int i = 0; i < num_test + num_train; i++) {
        for (int j = 0; j < num; j++) {
            float value = (float)mat_feature_pca[i].at<double>(0, j) / scale[j];
            mat_test.at<float>(0, j) = value;
        }
        res.push_back(svm->predict(mat_test));
    }
    // Get error rate
    count_error_train = 0;
    count_error_test = 0;
    for (int i = 0; i < num_train; i++) {
        if (res[i] != list_label[i]) {
            //cout << "No" << i << ": " << list_label[i] << "  " << res[i] << "  (*)" << endl;
            count_error_train++;
        } else {
            //cout << "No" << i << ": " << list_label[i] << "  " << res[i] << endl;
        }
    }
    for (int i = 0; i < num_test; i++) {
        if (res[i + num_train] != 1) {
            //cout << "No" << i + num_train << ": " << list_label[i] << "  " << res[i + num_train] << "  (*)" << endl;
            count_error_test++;
        } else {
            //cout << "No" << i + num_train << ": " << list_label[i] << "  " << res[i + num_train] << endl;
        }
    }
}



void Classifier::Print_Label() {
    cout << "num_train: " << num_train << " num_test: " << num_test << endl;
    for (int i = 0; i < num_train; i++) {
        cout << "list_train: " << list_label_train[i] << "  " << list_filename_train[i] << endl;
    }
    cout << endl;
    for (int i = 0; i < num_test; i++) {
        cout << "list_test: " << list_label_test[i] << "  " << list_filename_test[i] << endl;
    }
    cout << endl << "num_label: " << num_label << endl;
    for (int i = 0; i < num_label; i++) {
        cout << "label name: " << list_label_name[i] << " label num: " << list_num_per_label[i] << endl;
    }
    cout << endl;
}
void Classifier::Print_Error_Rate() {
    cout << "error_rate_total: " << 100 * (double)(count_error_train + count_error_test) / (double)(num_train + num_test) << " %" << endl;
    cout << "error_rate_train: " << 100 * (double)count_error_train / (double)num_train << " %" << endl;
    cout << "error_rate_test: " << 100 * (double)count_error_test / (double)num_test << " %" << endl;
}
void Classifier::Print_Stat(int mode) {
    if (mode == 0) {
        cout << "----------mat_feature----------" << endl;
        for (int i = 0; i < mat_feature.size(); i++) {
            cout << mat_feature[i] << endl;
        }
        cout << "----------mat_feature_average----------" << endl;
        for (int i = 0; i < mat_feature_average.size(); i++) {
            cout << mat_feature_average[i] << endl;
        }
    } else if (mode == 1) {
        cout << "----------mat_feature_pca----------" << endl;
        for (int i = 0; i < mat_feature_pca.size(); i++) {
            cout << mat_feature_pca[i] << endl;
        }
        cout << "----------mat_feature_average_pca----------" << endl;
        for (int i = 0; i < mat_feature_average_pca.size(); i++) {
            cout << mat_feature_average_pca[i] << endl;
        }
    } else if (mode == 2) {
        cout << "----------mat_feature_lda----------" << endl;
        for (int i = 0; i < mat_feature_lda.size(); i++) {
            cout << mat_feature_lda[i] << endl;
        }
        cout << "----------mat_feature_average_lda----------" << endl;
        for (int i = 0; i < mat_feature_average_lda.size(); i++) {
            cout << mat_feature_average_lda[i] << endl;
        }
    } else {
        cout << "ERROR" << endl << endl;
    }
}


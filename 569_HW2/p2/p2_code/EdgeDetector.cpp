//
// Created by Maggie on 10/10/15.
//

#include "EdgeDetector.h"

EdgeDetector::EdgeDetector(string filename, int new_height, int new_width, int new_byteperpixel) {
    Mat mat = ImgMatOperator::Mat_Raw_Read(filename, new_height, new_width, new_byteperpixel);
    height = new_height;
    width = new_width;
    byteperpixel = new_byteperpixel;
    mat_original = mat.clone();
}

EdgeDetector::EdgeDetector(Mat mat, int new_height, int new_width, int new_byteperpixel) {
    height = new_height;
    width = new_width;
    byteperpixel = new_byteperpixel;
    mat_original = mat.clone();
}

EdgeDetector::EdgeDetector(Img *pt_img, int new_height, int new_width, int new_byteperpixel) {
    height = new_height;
    width = new_width;
    byteperpixel = new_byteperpixel;
    size_t size = height * width * byteperpixel;
    Img image[size];
    memcpy(image, pt_img, size);
    if (byteperpixel == 1) {
        double d_image[size];
        int count = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                d_image[count] = (double) image[count];
                count++;
            }
        }
        mat_original = Mat(height, width, CV_64F, d_image).clone();
    }
    if (byteperpixel == 3) {
        double d_image[size];
        int count = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                for (int k = 0; k < byteperpixel; k++) {
                    if (k == 0) {
                        Img temp = image[count];
                        image[count] = image[count + 2];
                        image[count + 2] = temp;
                    }
                    d_image[count] = (double) image[count];
                    count++;
                }
            }
        }
        mat_original = Mat(height, width, CV_64FC3, d_image).clone();
    }
}

void EdgeDetector::ConvertRGB(Mat mat_color) {
    cout << "ConvertRGB" << endl;
    if (mat_color.channels() == 3) {
        double img_gray[height * width];
        int count = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                img_gray[count++] = mat_color.at<double>(i, j * 3) * 0.21
                                    + mat_color.at<double>(i, j * 3 + 1) * 0.72
                                    + mat_color.at<double>(i, j * 3 + 2) * 0.07;
        }
        mat_gray = Mat(height, width, CV_64F, img_gray).clone();
    } else {
        mat_gray = mat_color.clone();
    }
}

void EdgeDetector::Detector_Sobel(double threshold) {
    // 1. Check parameter
    if (threshold >= 1 || threshold <= 0) {
        cerr << "threshold " << threshold << " is invalid";
        exit(1);
    }

    // 2. Apply Sobel filter to x, y two dimensions
    // 2.1 Generate Sobel mask
    double filter[2][9] = {{-0.25, 0,    0.25,  -0.5, 0, 0.5, -0.25, 0,   0.25},
                           {-0.25, -0.5, -0.25, 0,    0, 0,   0.25,  0.5, 0.25}};
    Mat mat_gradient_x = Mat(height, width, CV_64F);
    Mat mat_gradient_y = Mat(height, width, CV_64F);
    // 2.2 Apply Sobel mask to x direction, and get gradient x
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double temp_total = 0;
            int *window = ImgMatOperator::Get_Window(i, j, height, width, 1);
            for (int m = window[0]; m <= window[1]; m++) {
                for (int n = window[2]; n <= window[3]; n++) {
                    temp_total += filter[0][3 * (1 + m - i) + (1 + n - j)] * mat_gray.at<double>(m, n);
                }
            }
            mat_gradient_x.at<double>(i, j) = temp_total;
        }
    }
    // 2.3 Apply Sobel mask to y direction, and get gradient y
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double temp_total = 0;
            int *window = ImgMatOperator::Get_Window(i, j, height, width, 1);
            for (int m = window[0]; m <= window[1]; m++) {
                for (int n = window[2]; n <= window[3]; n++) {
                    temp_total += filter[1][3 * (1 + m - i) + (1 + n - j)] * mat_gray.at<double>(m, n);
                }
            }
            mat_gradient_y.at<double>(i, j) = temp_total;
        }
    }
    // 2.4 Get magnitude image gradient_x, gradient_y
    Mat mat_magnitude = Mat(height, width, CV_64F);
    Mat mat_oritentation = Mat(height, width, CV_64F);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mat_magnitude.at<double>(i, j) =
                    sqrt(mat_gradient_x.at<double>(i, j) * mat_gradient_x.at<double>(i, j)
                         + mat_gradient_y.at<double>(i, j) * mat_gradient_y.at<double>(i, j));
        }
    }

    // 3. Normalize the magnitude matrix to 0 ~ 255
    double min = 10000;
    double max = -10000;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double temp = mat_magnitude.at<double>(i, j);
            if (temp < min)
                min = temp;
            if (temp > max)
                max = temp;
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            mat_magnitude.at<double>(i, j) = (mat_magnitude.at<double>(i, j) - min) * 255.0 / (max - min);
    }

    // 4. Use threshold to binary the edge and non-edge
    // 4.1 Sort the normalized matrix and get the intensity value by threshold
    vector<double> vector_temp;
    Mat mat_threshold = Mat(height, width, CV_64F);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            vector_temp.push_back(mat_magnitude.at<double>(i, j));
    }
    sort(vector_temp.begin(), vector_temp.end());
    double threshold_value = vector_temp[height * width * (1 - threshold)];
    // 4.2 Set black to edge, white to non-edge in the normalized image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (mat_magnitude.at<double>(i, j) > threshold_value)
                mat_threshold.at<double>(i, j) = 0;
            else
                mat_threshold.at<double>(i, j) = 1;
        }
    }

    // 5. non-maximum suprression
    Mat mat_suppress = Mat(height, width, CV_64F);
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            double temp_x = mat_gradient_x.at<double>(i, j);
            double temp_y = mat_gradient_x.at<double>(i, j);
            double temp_value;
            if (temp_x * temp_x >= temp_y * temp_y) {
                temp_value = mat_threshold.at<double>(i, j);
                if (temp_value >= mat_threshold.at<double>(i, j - 1)
                    && temp_value >= mat_threshold.at<double>(i, j + 1)) {
                    mat_suppress.at<double>(i, j) = 1;
                } else {
                    mat_suppress.at<double>(i, j) = 0;

                }
            } else {
                temp_value = mat_threshold.at<double>(i, j);
                if (temp_value >= mat_threshold.at<double>(i - 1, j)
                    && temp_value >= mat_threshold.at<double>(i + 1, j)) {
                    mat_suppress.at<double>(i, j) = 1;
                } else {
                    mat_suppress.at<double>(i, j) = 0;
                }
            }
        }
    }

    // 6. Print raw image for report
    Print_Raw(mat_original, "Cougar_original.raw", 0, 3);
    Print_Raw(mat_gray, "Cougar_gary.raw", 0, 1);
    Print_Raw(mat_gradient_x, "Cougar_gradient_x.raw", 1, 1);
    Print_Raw(mat_gradient_y, "Cougar_gradient_y.raw", 1, 1);
    Print_Raw(mat_magnitude, "Cougar_magnitude.raw", 0, 1);
    Print_Raw(mat_threshold, "Cougar_threshold_015.raw", 1, 1);
    Print_Raw(mat_suppress, "Cougar_suppression_015.raw", 1, 1);
}

void EdgeDetector::Detector_Canny(double coef_A, double coef_B) {
    // Apply Canny
    Mat mat_canny;
    Mat mat_temp = Mat(height, width, CV_8U);
    mat_gray.convertTo(mat_temp, CV_8U);
    Canny(mat_temp, mat_canny, coef_A * 255, coef_B * 255);
    // Convert the result to print raw image
    mat_canny.convertTo(mat_canny, CV_64F);
    Mat mat_temp2 = Mat::ones(height, width, CV_64F) * 255;
    mat_canny = mat_temp2 - mat_canny;
    string str = "Cougar_" + to_string((int)(10 * coef_A)) + "_" + to_string((int)(10 * coef_B)) + ".raw";
    Print_Raw(mat_canny, str, 0, 1);
}

//@Para mode: 1 normalization; 0 no normalization;
void EdgeDetector::Print_Raw(Mat mat, string filename, int mode, int byte) {
    if (mode == 1) {
        int cols = mat.cols;
        int rows = mat.rows;
        double min = 10000;
        double max = -10000;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                double temp = mat.at<double>(i, j);
                if (temp < min)
                    min = temp;
                if (temp > max)
                    max = temp;
            }
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++)
                mat.at<double>(i, j) = (mat.at<double>(i, j) - min) / (max - min);
        }
        mat = mat * 255.0;
    }
    if (byte == 1)
        ImgMatOperator::Mat_Raw_Write_Gray(mat, filename);
    if (byte == 3) {
        ImgMatOperator::Mat_Raw_Write_Color(mat, filename);
    }


}

void EdgeDetector::Print_Mat(Mat mat, string name) {
    int cols = mat.cols;
    int rows = mat.rows;
    double min = 10000;
    double max = -10000;
    cout << name << ": " << endl;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            cout << mat.at<double>(i, j) << ", ";
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double temp = mat.at<double>(i, j);
            if (temp < min) {
                min = temp;
            }
            if (temp > max) {
                max = temp;
            }
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat.at<double>(i, j) = (mat.at<double>(i, j) - min) / (max - min);
        }
    }
    imshow(name, mat);
    waitKey(0);
}

void EdgeDetector::Print_Detail() {
    cout << "Print_Detail" << endl;
    cout << "height: " << height << " width: " << width << " byteperpixel: " << byteperpixel << endl;

    imshow("mat_original", mat_original / 255.0);
    cout << "mat_original: " << endl;
    for (int i = 0; i < 10; i++) {
        cout << mat_original.at<double>(height - 1, i) << ", ";
    }
    cout << endl;
    waitKey(0);

    imshow("mat_gray", mat_gray / 255.0);
    cout << "mat_gray: " << endl;
    for (int i = 0; i < 10; i++) {
        cout << mat_gray.at<double>(height - 1, i) << ", ";
    }
    cout << endl;
    waitKey(0);
}




//
// Created by Maggie on 10/6/15.
//
#include "hw2_helper.h"
using namespace std;


/*
 * Prob2a. Sobel edge detector and non-maximal suppression
 * Convert RGB to gray
 * Sobel Filter
 * Normalization
 * Threshodls edge
 * Non-Max suppression
 *
 * Prob2b. Canny edge detector
 * Convert RGB to gray
 * Canny Filter (matlab/opencv)
 *
 * Prob2c. Structured edge
 * Structured edge > probability edge map
 * Threshold
 *
 * Prob2d. Performance evaluation
 */

const string FOLDER = "p2_image/";
const int WIDTH = 481;
const int HEIGHT = 321;
const int BYTEPERPIXEL = 3;

const string FILENAME1 = "p2_image/Farm.raw";
const string FILENAME2 = "p2_image/Cougar.raw";

class EdgeDetector {

public:
    Mat mat_original;
    int height;
    int width;
    int byteperpixel;

    Mat mat_gray;

    EdgeDetector(Mat mat) {
        mat_original = mat.clone();
    }

    EdgeDetector(Img *pt_img, int new_height, int new_width, int new_byteperpixel) {
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
                    d_image[count] = (double)image[count];
                    count++;
                }
            }
            mat_original = Mat(height, width, CV_64F, d_image).clone();
        } if (byteperpixel == 3) {
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
                        d_image[count] = (double)image[count];
                        count++;
                    }
                }
            }
            mat_original = Mat(height, width, CV_64FC3, d_image).clone();
        }
    }

    void ConvertRGB(Mat mat_color) {
        cout << "ConvertRGB" << endl;
        if (mat_color.channels() == 3) {
            double img_gray[height * width];
            int count = 0;
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    img_gray[count++] = mat_color.at<double>(i, j * 3) * 0.21
                            + mat_color.at<double>(i, j * 3 + 1) * 0.72
                            + mat_color.at<double>(i, j * 3 + 2) * 0.07;
                }
            }
            mat_gray = Mat(height, width, CV_64F, img_gray).clone();
        } else {
            mat_gray = mat_color.clone();
        }
    }

    void Filter_Sobel (double threshold) {
        // Parameter
        if (threshold > 1 || threshold < 0) {
            cerr << "threshold " << threshold << " is invalid";
            exit(1);
        }

        // Filter
        double filter[2][9] = {{-0.25, 0, 0.25, -0.5, 0, 0.5, -0.25, 0, 0.25},
                            {-0.25, -0.5, -0.25, 0, 0, 0, 0.25, 0.5, 0.25}};
        Mat mat_gradient_x = Mat(height, width, CV_64F);
        Mat mat_gradient_y = Mat(height, width, CV_64F);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                double temp_total = 0;
                int *window = ImgMatOperator::Get_Window(i, j, height, width, 1);
                for (int m = window[0]; m <= window[1]; m++) {
                    for (int n = window[2]; n <= window[3]; n++) {
                        temp_total += filter[0][3 * (1 + m - i) + (1 + n - j)] * mat_gray.at<double>(m, n);
                    }
                }
                mat_gradient_x.at<double> (i, j) = temp_total;
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                double temp_total = 0;
                int *window = ImgMatOperator::Get_Window(i, j, height, width, 1);
                for (int m = window[0]; m <= window[1]; m++) {
                    for (int n = window[2]; n <= window[3]; n++) {
                        temp_total += filter[1][3 * (1 + m - i) + (1 + n - j)] * mat_gray.at<double>(m, n);
                    }
                }
                mat_gradient_y.at<double> (i, j) = temp_total;
            }
        }
        Mat mat_magnitude = Mat(height, width, CV_64F);
        Mat mat_oritentation = Mat(height, width, CV_64F);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                mat_magnitude.at<double>(i, j) = sqrt(mat_gradient_x.at<double>(i, j) * mat_gradient_x.at<double>(i, j)
                                                      + mat_gradient_y.at<double>(i, j) *
                                                        mat_gradient_y.at<double>(i, j));
            }
        }

        // normalization
        double min = 10000;
        double max = -10000;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                double temp = mat_magnitude.at<double> (i, j);
                if (temp < min) {
                    min = temp;
                }
                if (temp > max) {
                    max = temp;
                }
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                mat_magnitude.at<double>(i, j) = (mat_magnitude.at<double>(i, j) - min) * 255.0 / (max - min);
            }
        }

        // Threshold
        vector<double> vector_temp;
        Mat mat_threshold = Mat(height, width, CV_64F);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                vector_temp.push_back(mat_magnitude.at<double>(i, j));
            }
        }
        sort(vector_temp.begin(), vector_temp.end());
        double threshold_value = vector_temp[height * width * (1 - threshold)];
        cout << "threshold_value: " << threshold_value << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (mat_magnitude.at<double>(i, j) >= threshold_value) {
                    mat_threshold.at<double>(i, j) = 0;
                } else {
                    mat_threshold.at<double>(i, j) = 1;
                }
            }
        }

        // non-maximum suprression
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
        Print_Mat(mat_gray, "mat_gray");
        Print_Mat(mat_gradient_x, "mat_gradient_x");
        Print_Mat(mat_gradient_y, "mat_gradient_y");
        Print_Mat(mat_magnitude, "mat_magnitude");
        Print_Mat(mat_threshold, "mat_threshold");
        Print_Mat(mat_suppress, "mat_suppress");
    }

    void Print_Mat(Mat mat, string name) {
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
                double temp = mat.at<double> (i, j);
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

    void Print_Detail() {
        cout << "Print_Detail" << endl;
        cout << "height: " << height << " width: " << width << " byteperpixel: " << byteperpixel << endl;

        imshow("mat_original", mat_original / 255.0);
        cout << "mat_original: " << endl;
        for (int i = 0; i < 10; i++) {
            cout << mat_original.at<double>(height - 1, i) << ", ";
        }
        waitKey(0);

        imshow("mat_gray", mat_gray / 255.0);
        cout << "mat_gray: " << endl;
        for (int i = 0; i < 10; i++) {
            cout << mat_gray.at<double>(height - 1, i) << ", ";
        }
        waitKey(0);
    }

    void Detector_Canny (double coef_A, double coef_B) {
        Mat mat_canny  = Mat(height, width, CV_8U);
        Mat mat_temp = Mat(height, width, CV_8U);

        mat_gray.convertTo(mat_temp, CV_8U);
        Canny(mat_temp, mat_canny, coef_A * 255, coef_B * 255);
        string str = "Canny A = " + to_string(coef_A) + " B = " + to_string(coef_B);
        imshow(str, mat_canny);
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 20; j++) {
                cout << mat_canny.at<double>(i, j) << ", ";
            }
            cout << endl;
        }
        waitKey(0);
    }
};

void prob2a() {
    //ImgMatOperator img_op;
    Img *img = ImgMatOperator::Img_Raw_Read (FILENAME1, HEIGHT, WIDTH, BYTEPERPIXEL);
    EdgeDetector ed = EdgeDetector(img, HEIGHT, WIDTH, BYTEPERPIXEL);
    ed.ConvertRGB(ed.mat_original);
    //ed.Filter_Sobel(0.1);
    //ed.Filter_Sobel(0.15);
    //ed.Print_Detail();

    Img *img2 = ImgMatOperator::Img_Raw_Read (FILENAME2, HEIGHT, WIDTH, BYTEPERPIXEL);
    EdgeDetector ed2 = EdgeDetector(img2, HEIGHT, WIDTH, BYTEPERPIXEL);
    ed2.ConvertRGB(ed2.mat_original);
    //ed2.Filter_Sobel(0.1);
    //ed2.Filter_Sobel(0.15);
}

void prob2b() {
    Img *img = ImgMatOperator::Img_Raw_Read (FILENAME1, HEIGHT, WIDTH, BYTEPERPIXEL);
    EdgeDetector ed = EdgeDetector(img, HEIGHT, WIDTH, BYTEPERPIXEL);
    ed.ConvertRGB(ed.mat_original);
    ed.Detector_Canny(0.3, 0.6);
    ed.Detector_Canny(0.2, 0.7);
    ed.Detector_Canny(0.2, 0.5);
    ed.Detector_Canny(0.4, 0.7);
    ed.Detector_Canny(0.4, 0.5);


    Img *img2 = ImgMatOperator::Img_Raw_Read (FILENAME2, HEIGHT, WIDTH, BYTEPERPIXEL);
    EdgeDetector ed2 = EdgeDetector(img2, HEIGHT, WIDTH, BYTEPERPIXEL);
    ed2.ConvertRGB(ed2.mat_original);
    ed2.Detector_Canny(0.3, 0.6);
    ed2.Detector_Canny(0.2, 0.7);
    ed2.Detector_Canny(0.2, 0.5);
    ed2.Detector_Canny(0.4, 0.7);
    ed2.Detector_Canny(0.4, 0.5);
}



int main(int argc, char *argv[])
{
    cout << "Problem 2" << endl;
    //prob2a();
    //prob2b();
    return 0;
}
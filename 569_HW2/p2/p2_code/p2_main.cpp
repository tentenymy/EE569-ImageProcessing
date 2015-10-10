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

/*
 * Problem2. Edge Detection
 * Prob2a. Sobel edge detector and non-maximal suppression
 * prob2b. Canny edge detector
 * prob2c. Structured edge (Matlab code)
 * prob2d. Performance evaluation
 */

#include "edge_detector.h"
using namespace std;

const string FOLDER = "p2_image/";
const int WIDTH = 481;
const int HEIGHT = 321;
const int BYTEPERPIXEL = 3;
const string NAME[2] = {"Farm", "Cougar"};
const string FILENAME[2] =
        {"p2_image/Farm.raw", "p2_image/Cougar.raw"};

// Prob2a. Sobel edge detector and non-maximal suppression
void prob2a() {
    cout << "Problem 2a" << endl;
    for (int i = 0; i < 1; i++) {
        EdgeDetector ed = EdgeDetector(FILENAME[i], HEIGHT, WIDTH, BYTEPERPIXEL);
        ed.ConvertRGB();
        ed.Detector_Sobel(0.1);
        ed.Detector_Sobel(0.15);
    }
}

// Prob2b. Canny edge detector
void prob2b() {
    cout << "Problem 2b" << endl;
    for (int i = 1; i < 2; i++) {
        EdgeDetector ed = EdgeDetector(FILENAME[i], HEIGHT, WIDTH, BYTEPERPIXEL);
        ed.ConvertRGB();
        ed.Detector_Canny(0.3, 0.6);
        ed.Detector_Canny(0.2, 0.7);
        ed.Detector_Canny(0.2, 0.5);
        ed.Detector_Canny(0.4, 0.7);
        ed.Detector_Canny(0.4, 0.5);
    }
}

// Prob2d. Performance evaluation
void prob2d() {
    cout << "Problem 2d" << endl;
    for (int i = 0; i < 2; i++) {
        cout << endl << NAME[i] << endl;
        vector<string> filename_gt;
        for (int j = 0; j < 5; j++) {
            string temp_str = FOLDER + NAME[i] + "_GT" + to_string(j+1) + ".raw";
            filename_gt.push_back(temp_str);
        }
        cout << endl << "p2a: Sobel" << endl;
        EdgeDetector::Evaluation("p2a_result/" + NAME[i] + "_suppression_010.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2a_result/" + NAME[i] + "_suppression_015.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);

        cout << endl << "p2b: Canny" << endl;
        EdgeDetector::Evaluation("p2b_result/" + NAME[i] + "_3_6.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2b_result/" + NAME[i] + "_2_7.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2b_result/" + NAME[i] + "_2_5.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2b_result/" + NAME[i] + "_4_7.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2b_result/" + NAME[i] + "_4_5.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);

        cout << endl << "p2c: SE" << endl;
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_02.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_03.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_04.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_05.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_06.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_07.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_08.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_09.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
        EdgeDetector::Evaluation("p2c_result/" + NAME[i] + "_SE_10.raw", filename_gt, HEIGHT, WIDTH, BYTEPERPIXEL);
    }
}

int main(int argc, char *argv[])
{
    cout << "Problem 2" << endl;
    prob2a();
    prob2b();
    prob2d();
    return 0;
}
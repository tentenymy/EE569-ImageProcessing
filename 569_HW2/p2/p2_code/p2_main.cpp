//
// Created by Maggie on 10/6/15.
//
#include "EdgeDetector.h"
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
 * Canny Filter (open)
 *
 * Prob2c. Structured edge (matlab)
 * Structured edge > probability edge map
 * Threshold
 *
 * Prob2d. Performance evaluation
 */

const string FOLDER = "p2_image/";
const int WIDTH = 481;
const int HEIGHT = 321;
const int BYTEPERPIXEL = 3;
const string FILENAME[2] = {"p2_image/Farm.raw", "p2_image/Cougar.raw"};



void prob2a() {
    for (int i = 0; i < 2; i++) {
        EdgeDetector ed = EdgeDetector(FILENAME[i], HEIGHT, WIDTH, BYTEPERPIXEL);
        ed.ConvertRGB(ed.mat_original);
        ed.Detector_Sobel(0.1);
        ed.Detector_Sobel(0.15);
    }
}

void prob2b() {
    for (int i = 1; i < 2; i++) {
        EdgeDetector ed = EdgeDetector(FILENAME[i], HEIGHT, WIDTH, BYTEPERPIXEL);
        ed.ConvertRGB(ed.mat_original);
        ed.Detector_Canny(0.3, 0.6);
        ed.Detector_Canny(0.2, 0.7);
        ed.Detector_Canny(0.2, 0.5);
        ed.Detector_Canny(0.4, 0.7);
        ed.Detector_Canny(0.4, 0.5);
    }
}


void prob2c() {
    Img *pt_img = ImgMatOperator::Img_Raw_Read (FILENAME[0], HEIGHT, WIDTH, BYTEPERPIXEL);
    char filename[] = "test.raw";
    ImgMatOperator img_op;
    Img img[HEIGHT * WIDTH * BYTEPERPIXEL];
    memcpy(img, pt_img, HEIGHT * WIDTH * BYTEPERPIXEL);
    img_op.Img_Raw_Write(filename, img, HEIGHT, WIDTH, BYTEPERPIXEL);
}

void prob2d() {

}



int main(int argc, char *argv[])
{
    cout << "Problem 2" << endl;
    prob2a();
    prob2b();
    //prob2c();
    return 0;
}
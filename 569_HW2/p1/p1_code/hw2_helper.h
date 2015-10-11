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

#ifndef HW2_HELPER_H
#define HW2_HELPER_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <cstring>
#include <opencv2/opencv.hpp>


typedef unsigned char Img;

using namespace cv;
using namespace std;

class ImgMatOperator {
public:
    ImgMatOperator();

    Img* Img_Raw_Read (char *filename, int height, int width, int byteperpixel);
    static Img* Img_Raw_Read (string, int, int, int);
    void Img_Raw_Write (char* filename, Img *image, int height, int width, int byteperpixel);
    void Img_File_Print (Img *image, string filename, int height, int width, int byteperpixel);
    Mat Img_To_Mat_Convert (Img *image, int height, int width, int byteperpixel);

    void Mat_File_Print (Mat mat, string filename);
    void Mat_XML_Print(Mat mat, string filename);
    void Mat_Raw_Write(Mat mat, string filename);
    Mat Mat_Raw_Read(string filename, int height, int width, int byteperpixel);

    static int *Get_Window(int i, int j, int height, int width, int half_window);
    void Test();
};


#endif //HW2_P1A_H


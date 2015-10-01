//
// Created by Maggie on 9/30/15.
//

#ifndef HW2_P1A_H
#define HW2_P1A_H

#endif //HW2_P1A_H

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

class ImgMatOperator
{
public:
    ImgMatOperator();


    Img* Img_Raw_Read (char *filename, int height, int width, int byteperpixel);

    void Img_Raw_Write (char* filename, Img *image, int height, int width, int byteperpixel);

    void Mat_File_Print (Mat mat, string filename);

    void Img_File_Print (Img *image, string filename, int height, int width, int byteperpixel);

    Mat Img_To_Mat_Convert (Img *image, int height, int width, int byteperpixel);

    void Mat_Raw_Write_Gray(string filename, Mat mat);
};

void Test();
//
// Created by Maggie on 9/5/15.
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>

#ifndef HW1_HW1_HELPER_H
#define HW1_HW1_HELPER_H

#endif //HW1_HW1_HELPER_H

using namespace std;

const int MAXSIZE = 1024;
const int MAXBYTE = 3;
const int DEFAULTBYTE = 3;
const int DEFAULTSIZE = 512;
const int COLORSIZE = 256;

const int PLOT_X_SIZE = 32;
const int PLOT_Y_SIZE = 500;

const int DEBUG = 1;


// This class is like head information of regular image.
class Info
{
public:
    FILE *file;
    const char* filename;
    int byteperpixel;
    int width;
    int height;

    Info();
    Info(const char* new_filename, int new_byteperpixel, int new_weight, int new_height);
    void Info_Print();// print the information for me.
};

// Print all the image to a file, by changing every pixel (R -> G -> B) to interger
void Image_Print_By_Interger(unsigned char *, Info *, string);

// Print certain line to a file.
void Image_Plot_Line(short *, int, string);

// Print statistic of eachcolor to a file and do a simple plot
void Image_Plot_All_Line(unsigned char *, Info *, string);

// Check whether in the debug mod, if no, exit!
void Check_Debug();
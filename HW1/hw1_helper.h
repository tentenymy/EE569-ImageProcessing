//
// Created by Maggie on 9/5/15.
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <cstring>

#ifndef HW1_HW1_HELPER_H
#define HW1_HW1_HELPER_H

#endif //HW1_HW1_HELPER_H

using namespace std;

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;

const int MAX_SIZE = 1024;
const int MAX_BYTE = 3;
const int COLOR_BYTE = 3;
const int GRAY_BYTE = 1;
const int DEFAULT_SIZE = 512;
const int COLOR_SIZE = 256;


// For debug or compile
const int DEBUG = 1;
const char FOLDER_READ[]= "Image_read/";
const char FOLDER_WRITE[]= "Image_write/";
const string FOLDER_DEBUG = "Image_write/";

const int PLOT_X_SIZE = 32;
const int PLOT_Y_SIZE = 500;


// This class is like head information of regular image.
class Info
{
public:
    FILE *file;
    const char* filename_read;
    const char* filename_write;
    int byteperpixel;
    int width;
    int height;

    Info();
    Info(const char* new_filename_read, const char * new_filename_write, int new_byteperpixel, int new_weight, int new_height);
    Info(int, char *[]);
    void Info_Print();// print the information for me.
    void Info_File_Read();
    void Info_File_Write();
    void Info_File_Close(); //close this->file;
};

// Print all the image to a file, by changing every pixel (R -> G -> B) to interger
void Image_Print_By_Interger(unsigned char *, Info *, string);

// Print certain line to a file.
void Image_Plot_Gray_Line(unsigned char *, Info *, string);

// Print statistic of eachcolor to a file and do a simple plot
void Image_Plot_All_Line(unsigned char *, Info *, string);

// Check whether in the debug mod, if no, exit!
void Check_Debug();
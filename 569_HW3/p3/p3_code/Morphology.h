/* EE569 Homework Assignment #3
 * Date: Nov 1, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * Email: meiyiyan@usc.edu
 *
 * Problem3. Morphological Processing
 *
 * p3_main.cpp
 * Image.h
 * Image.cpp
 * Morphology.h
 * Morphology.cpp
 */

#ifndef P3_MORPHOLOGY_H
#define P3_MORPHOLOGY_H
#include "Image.h"

///////////////////////////////////////
///////////// Data Declaration /////////////
///////////////////////////////////////
typedef int Dataset[9];
const int PATTERN_S1 = 58;
const int PATTERN_T1 = 46;
const int PATTERN_K1 = 40;
const int PATTERN_S2 = 332;
const int PATTERN_T2 = 332;
const int PATTERN_K2 = 348;
const string FILE_S1 = "p3_image/Pattern_S1.txt";
const string FILE_T1 = "p3_image/Pattern_T1.txt";
const string FILE_K1 = "p3_image/Pattern_K1.txt";
const string FILE_S2 = "p3_image/Pattern_S2.txt";
const string FILE_T2 = "p3_image/Pattern_T2.txt";
const string FILE_K2 = "p3_image/Pattern_K2.txt";


/////////////////////////////////////////////
///////////// Class Declaration /////////////
/////////////////////////////////////////////
class Morphology {
public:
    Image image;
    Dataset *filter = NULL;
    Dataset *filter2 = NULL;
    int is_binary = 0;


///////////////////////////////////////
///////////// Constructor /////////////
///////////////////////////////////////
    Morphology();
    Morphology(Image *new_image);
    Morphology(int new_row, int new_col, int new_byte, ImgPixel *pt_img);
    ~Morphology();


////////////////////////////////////////////
///////////// S/T/K Morphology /////////////
////////////////////////////////////////////
// @para: filename: Conditional/Unconditonal Pattern file
// list of filename is writen above
// @para: size: the number of patterns in the file
    int Apply_Hit_Miss(int size_filter1, int size_filter2);
    int Operator_Hit_Miss(string filename1, string filename2, int size1, int size2);


///////////////////////////////////////////////////
///////////// Dilate/Erode Morphology /////////////
///////////////////////////////////////////////////
// @para filter: structure to filter the image (3 * 3)
    int Operator_Dilate(Dataset filter);
    int Operator_Erode(Dataset filter);
    int Operator_Open(Dataset filter1, Dataset filter2);
    int Operator_Close(Dataset filter1, Dataset filter2);
    int Operator_Filter(Dataset filter);


///////////////////////////////////////////////
///////////// Counting and Display/////////////
///////////////////////////////////////////////
    int Convert_Black_Image();
    int Write(string filename);
    int Count_Pathway();
    int Count_Pixel();


private:
///////////////////////////////////////////////////
///////////// S/T/K Morphology Private/////////////
///////////////////////////////////////////////////
// @para data1: neighbor of pixels
// @para data2: filter pattern
// @para: filename: Conditional/Unconditonal Pattern file
// list of filename is writen above
// @para: size: the number of patterns in the file
    int Initialize_Pattern_Conditional(string filename, int size);
    int Initialize_Pattern_Unconditional(string filename, int size);
    int Compare_Dataset_Conditional(Dataset data1, Dataset data2);
    int Compare_Dataset_Unconditional(Dataset data1, Dataset data2);

///////////////////////////////////////////
///////////// Helper Function /////////////
///////////////////////////////////////////
    int Convert_to_Binary_Image();
    int Convert_to_Grayscale_Image();
};

#endif //P3_MORPHOLOGY_H

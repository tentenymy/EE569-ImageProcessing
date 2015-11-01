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

#include "Image.h"
#include "Morphology.h"

//////////////////////////////////////
///////////// Declaration /////////////
//////////////////////////////////////
void Test_Shrinking(string filename_read, string filename_write, int row, int col);
void Test_Thinning(string filename_read, string filename_write, int row, int col);
void Test_Skeletonizing(string filename_read, string filename_write, int row, int col);
void Find_Nails();
void Find_Holes();
void Find_Objects();
void Apply_Thinning(int number_close, int number_dilate);
void Apply_Skeletonizing(int number_close, int number_dilate);
void prob3a();
void prob3b();


///////////////////////////////////////
///////////// Application /////////////
///////////////////////////////////////
void Test_Shrinking(string filename_read, string filename_write, int row, int col) {
    Image image = Image(row, col, 1, filename_read);
    Morphology op = Morphology(&image);
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write(filename_write);
}

void Test_Thinning(string filename_read, string filename_write, int row, int col) {
    Image image = Image(row, col, 1, filename_read);
    Morphology op = Morphology(&image);
    op.Operator_Hit_Miss(FILE_T1, FILE_T2, PATTERN_T1, PATTERN_T2);
    op.Write(filename_write);
}

void Test_Skeletonizing(string filename_read, string filename_write, int row, int col) {
    Image image = Image(row, col, 1, filename_read);
    Morphology op = Morphology(&image);
    op.Operator_Hit_Miss(FILE_K1, FILE_K2, PATTERN_K1, PATTERN_K2);
    op.Write(filename_write);
}

void Find_Nails() {
    // Initialization
    cout << "Find Nails" << endl;
    Image image = Image(108, 91, 1, "p3_image/Horseshoe.raw");
    Morphology op = Morphology(&image);

    // Pre-processing: Erode
    Dataset filter1 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    op.Operator_Erode(filter1);
    op.Write("Find_Nails_1Erode.raw");

    // Shrinking
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write("Find_Nails_2Shrink.raw");

    // Nail Filter
    Dataset filter2 = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    op.Operator_Filter(filter2);
    op.Write("Find_Nails_3Filter.raw");
    op.Count_Pixel();
}

void Find_Holes() {
    // Initialization
    cout << "Find Holes" << endl;
    Image image = Image(108, 91, 1, "p3_image/Horseshoe.raw");
    Morphology op = Morphology(&image);

    // Pre-processing: Convert image white to black
    op.Convert_Black_Image();
    op.Write("Find_Holes_1Black.raw");

    // Shrinking
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write("Find_Holes_2Shrink.raw");

    // Nail Filter
    Dataset filter1 = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    op.Operator_Filter(filter1);
    op.Write("Find_Holes_3Filter.raw");
    op.Count_Pixel();
}

void Find_Objects() {
    // Initialization
    cout << "Find Objects" << endl;
    Image image = Image(108, 91, 1, "p3_image/Horseshoe.raw");
    Morphology op = Morphology(&image);

    // Pre-processing: Convert image white to black
    Dataset filter1 = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Dataset filter2 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    Dataset filter3 = {0, 1, 0, 0, 1, 0, 0, 0, 0};
    Dataset filter4 = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    op.Operator_Open(filter1, filter2);
    op.Operator_Dilate (filter3);
    op.Write("Find_Object_1OpenDilate.raw");


    // Shrinking
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write("Find_Object_2Shrinking.raw");
    int res = op.Count_Pathway();
}

void Apply_Thinning(int number_close, int number_dilate) {
    string filename = "p3_image/Horse1.raw";
    int row = 275;
    int col = 360;
    Image image = Image(row, col, 1, filename);
    Morphology op = Morphology(&image);

    // Pre-processing
    Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Dataset filter2 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    for (int i = 0; i < number_close; i++)
        op.Operator_Close(filter, filter2);
    for (int i = 0; i < number_dilate; i++)
        op.Operator_Dilate(filter);

    // Thinning
    op.Operator_Hit_Miss(FILE_T1, FILE_T2, PATTERN_T1, PATTERN_T2);
    string filename_write = "Thinning_" + to_string(number_close) + "_" + to_string(number_dilate) + ".raw";
    op.Write(filename_write);
}

void Apply_Skeletonizing(int number_close, int number_dilate) {
    string filename = "p3_image/Horse1.raw";
    int row = 275;
    int col = 360;
    Image image = Image(row, col, 1, filename);
    Morphology op = Morphology(&image);

    // Pre-processing
    Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Dataset filter2 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    for (int i = 0; i < number_close; i++)
        op.Operator_Close(filter, filter2);
    for (int i = 0; i < number_dilate; i++)
        op.Operator_Dilate(filter);

    // Thinning
    op.Operator_Hit_Miss(FILE_K1, FILE_K2, PATTERN_K1, PATTERN_K2);
    string filename_write = "Skeletonizing_" + to_string(number_close) + "_" + to_string(number_dilate) + ".raw";
    op.Write(filename_write);
}


//////////////////////////////////////////////////
///////////// Problem Implementation /////////////
//////////////////////////////////////////////////
void prob3a() {
    cout << "Problem 3a" << endl;
    string filename = "p3_image/Horseshoe.raw";
    int row = 108;
    int col = 91;
    int number = 0;
    switch(number) {
        case 0:
            Find_Nails();
            Find_Holes();
            Find_Objects();
            break;
        case 1: // Test
            Test_Shrinking(filename, "Test_Shrinking.raw", row, col);
            Test_Thinning(filename, "Test_Thinning.raw", row, col);
            Test_Skeletonizing(filename, "Test_Skeletonizing.raw", row, col);
            break;
    }
}

void prob3b() {
    cout << "Problem 3b" << endl;
    int number = 0;
    switch(number) {
        case 0:
            Apply_Thinning(0, 0);
            Apply_Skeletonizing(0, 0);
            Apply_Thinning(2, 4);
            Apply_Skeletonizing(2, 4);
            break;
        case 1: // Test
            Apply_Thinning(2, 0);
            Apply_Thinning(2, 1);
            Apply_Thinning(2, 2);
            Apply_Thinning(2, 3);
            Apply_Thinning(2, 4);
            Apply_Thinning(2, 5);
            break;
        case 2:
            Apply_Thinning(0, 2);
            Apply_Thinning(1, 2);
            Apply_Thinning(2, 2);
            Apply_Thinning(3, 2);
            Apply_Thinning(4, 2);
            Apply_Thinning(5, 2);
            break;
        case 3:
            Apply_Skeletonizing(2, 0);
            Apply_Skeletonizing(2, 1);
            Apply_Skeletonizing(2, 2);
            Apply_Skeletonizing(2, 3);
            Apply_Skeletonizing(2, 4);
            Apply_Skeletonizing(2, 5);
            //break;
        case 4:
            Apply_Skeletonizing(0, 2);
            Apply_Skeletonizing(1, 2);
            Apply_Skeletonizing(2, 2);
            Apply_Skeletonizing(3, 2);
            Apply_Skeletonizing(4, 2);
            Apply_Skeletonizing(5, 2);
            break;
    }
}


/////////////////////////////////////////
///////////// Main Function /////////////
/////////////////////////////////////////
int main() {
    cout << "Homework 3.3" << endl;
    prob3a();
    prob3b();
    return 0;
}

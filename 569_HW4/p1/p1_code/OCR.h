/* EE569 Homework Assignment #4
 * Date: Noverber 29, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * email: meiyiyan@usc.edu
 * Problem1. OCR
 *
 * Main function:
 * p1_main.cpp
 *
 * Class OCR:
 * OCR.h
 * OCR.cpp
 *
 * Class Image:
 * image.h
 * image.cpp
 *
 * Class Morphology:
 * Morphology.h
 * Morphology.cpp
 */
#ifndef P1_OCR_H
#define P1_OCR_H

#include "Image.h"
#include "Morphology.h"

typedef int Bit_Quad[4];

class OCR {
public:
    Image image;
    Image image_test;
    int pathway_number;
    int pathway_number_test;
    string *label;
    string filename_train;
    string filename_test;
    int label_row;
    int label_col;
    Bit_Quad bit_quad[10];

/////////////////////////////////////////
////////////// Constructor //////////////
/////////////////////////////////////////
    OCR(int new_row, int new_col, int new_byte, string filename, int new_pathway_number, string *new_label);

//////////////////////////////////////
////////////// Training //////////////
//////////////////////////////////////
    void Train(string filename, int new_row, int new_col);

    void Segment_Train(string filename, int new_row, int new_col);

    void Feature_Train(string filename, int new_row, int new_col);

    string Classifier(double feature_euler, double feature_symmetry_X, double feature_symmetry_Y, double feature_moment_X, double feature_moment_Y, double feature_area, double feature_perimeter);

/////////////////////////////////////
////////////// Testing //////////////
/////////////////////////////////////
    void Test(int new_row, int new_col, int new_byte, string filename_test, string *label_test, int threshold1, int threshold2);

    void Segment_Test(string filename_test, Image *new_image, int threshold1, int threshold2);

    void Feature_Test_Symmetry(string filename_test, vector<double>* vector_symmetry_X, vector<double>* vector_symmetry_Y);

    void Feature_Test(string filename, string *new_label_test, vector<double>* vector_symmetry_X, vector<double>* vector_symmetry_Y);

    void Read_Test(int new_row, int new_col, int new_byte, string filename);

    void Process_Test(string filename_test, string *new_label_test);

////////////////////////////////////////
////////////// Preprocess //////////////
////////////////////////////////////////
    void Preprocess_Test1(string filename_test);

    void Preprocess_Test2(string filename_test);

    void Preprocess_Test3(string filename_test);

    void Preprocess_Test4(string filename_test);

private:
//////////////////////////////////////////////
////////////// Feature Analysis //////////////
//////////////////////////////////////////////
    double Feature_Area(string filename, int num);

    double Feature_Perimeter(string filename, int num);

    double Feature_Circularity(string filename, int num, double feature_area, double feature_perimeter);

    double Feature_Symmetry_Y(string filename, int num, double feature_area);

    double Feature_Symmetry_X(string filename, int num, double feature_area);

    double Feature_Euler(string filename, int num);

    double Feature_Moment_X(string filename, int num, double feature_area);

    double Feature_Moment_Y(string filename, int num, double feature_area);

/////////////////////////////////////////////
////////////// Helper Function //////////////
/////////////////////////////////////////////
    void Initial_Bit_Quad();

    int Compare_Bit_Quad(Bit_Quad bitquad1, Bit_Quad bitquad2);

    void Convert_To_Binary_Turn(Image* new_image);

    void Convert_To_Binary(Image* new_image);

    void Convert_To_Gray(Image* new_image);
};
#endif //P1_OCR_H

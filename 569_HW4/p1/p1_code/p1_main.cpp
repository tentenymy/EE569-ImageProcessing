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

#include <iostream>
#include "OCR.h"
using namespace std;

void Prob1a() {
    int row = 256;
    int col = 512;
    int pathway_number = 18;
    string label[18] = {"S", "P", "E", "D", "L", "I", "M", "T", "0", "1", "2", "3", "6", "8", "9", "4", "5", "7"};
    OCR ocr = OCR(row, col, 3, "p1_image/Training.raw", pathway_number, label);
    ocr.Train("p1_result/label", 100, 100);
}

void Prob1b(int number) {
    if (number == 0) {
        int row = 256;
        int col = 512;
        int row_test = 759;
        int col_test = 620;
        string filename_train = "p1_image/Training.raw";
        string filename_test = "p1_image/Test_ideal1.raw";
        string filename_index_train = "p1_result/label";
        string filename_index_test = "p1_result/Test1";

        int pathway_number = 18;
        string label[18] = {"S", "P", "E", "D", "L", "I", "M", "T", "0", "1", "2", "3", "6", "8", "9", "4", "5", "7"};
        string label_test[12] = {"E", "S", "E", "D", "P", "L", "I", "M", "I", "T", "0", "7"};

        OCR ocr = OCR(row, col, 3, filename_train, pathway_number, label);
        ocr.Train(filename_index_train, 100, 100);
        ocr.Read_Test(row_test, col_test, 3, filename_test);
        ocr.Preprocess_Test1(filename_index_test);
        ocr.Test(row_test, col_test, 3, filename_index_test, label_test, 10, 400);
    } else {
        int row = 256;
        int col = 512;
        int row_test = 400;
        int col_test = 250;
        string filename_train = "p1_image/Training.raw";
        string filename_test = "p1_image/Test_ideal2.raw";
        string filename_index_train = "p1_result/label";
        string filename_index_test = "p1_result/Test2";

        int pathway_number = 18;
        string label[18] = {"S", "P", "E", "D", "L", "I", "M", "T", "0", "1", "2", "3", "6", "8", "9", "4", "5", "7"};
        string label_test[12] = {"S", "P", "E", "E", "D", "L", "I", "M", "I", "T", "3", "1"};

        OCR ocr = OCR(row, col, 3, filename_train, pathway_number, label);
        ocr.Train(filename_index_train, 100, 100);
        ocr.Read_Test(row_test, col_test, 3, filename_test);
        ocr.Preprocess_Test2(filename_index_test);
        ocr.Test(row_test, col_test, 3, filename_index_test, label_test, 5, 200);
    }
}

void Prob1c() {
    int row = 256;
    int col = 512;
    int row_test = 360;
    int col_test = 267;
    string filename_train = "p1_image/Training.raw";
    string filename_test = "p1_image/Test_night.raw";
    string filename_index_train = "p1_result/label";
    string filename_index_test = "p1_result/Test3";

    int pathway_number = 18;
    string label[18] = {"S", "P", "E", "D", "L", "I", "M", "T", "0", "1", "2", "3", "6", "8", "9", "4", "5", "7"};
    string label_test[13] = {"S", "P", "E", "E", "D", "T", "M", "I", "I", "L", "5", "9", "1"};

    OCR ocr = OCR(row, col, 3, filename_train, pathway_number, label);
    ocr.Train(filename_index_train, 100, 100);
    ocr.Read_Test(row_test, col_test, 3, filename_test);
    ocr.Preprocess_Test3(filename_index_test);
    ocr.Test(row_test, col_test, 3, filename_index_test, label_test, 5, 90);
}

void Prob1c2() {
    int row = 256;
    int col = 512;
    int row_test = 361;
    int col_test = 215;
    string filename_train = "p1_image/Training.raw";
    string filename_test = "p1_image/Test_shade.raw";
    string filename_index_train = "p1_result/label";
    string filename_index_test = "p1_result/Test4";

    int pathway_number = 18;
    string label[18] = {"S", "P", "E", "D", "L", "I", "M", "T", "0", "1", "2", "3", "6", "8", "9", "4", "5", "7"};
    string label_test[21] = {"D", "E", "E", "P", "S", "T", "I", "M", "I", "L", "5", "2", "V", "R", "Y", "O", "E", "E", "D", "S", "P"};
    OCR ocr = OCR(row, col, 3, filename_train, pathway_number, label);
    ocr.Train(filename_index_train, 100, 100);
    ocr.Read_Test(row_test, col_test, 3, filename_test);
    ocr.Preprocess_Test4(filename_index_test);
    ocr.Test(row_test, col_test, 3, filename_index_test, label_test, 14, 120);
}

int main() {
    cout << "PROBLEM 1!" << endl;
    int number = 0;
    switch(number) {
        case 0:
            //Prob1a();
            Prob1b(0);
            Prob1b(1);
            Prob1c();
            Prob1c2();
            break;
        case 1:
            Prob1c();
            break;
        case 2:
            Prob1c2();
            break;
        default:
            break;
    }
    return 0;
}
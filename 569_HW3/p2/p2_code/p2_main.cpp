/* EE569 Homework Assignment #3
 * Date: Nov 1, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * Email: meiyiyan@usc.edu
 *
 * Problem2. Digital Half-toning
 *
 * p2_main.cpp
 * Image.h
 * Image.cpp
 * Halftoning.h
 * Halftoning.cpp
 */

#include "Halftoning.h"
using namespace std;

///////////////////////////////////
/////////// Declaration ///////////
///////////////////////////////////
// @para int size: Dithering index matrix size
// @para int color_number: different color number in each range
void Dithering(int size, int color_number);

// @para int mode
// 0 Floyd-Steinberg
// 1 JJN
// 2 Stucki
void Error_Diffusion(int mode);

// @para int mode
// 0 Floyd-Steinberg
// 1 JJN
// 2 Stucki
void Scalar_Color_Halftoning(int mode);

// @para int mode
// 0 Floyd-Steinberg
// 1 JJN
// 2 Stucki
void Vector_Color_Halftoning(int mode);

void prob2a();
void prob2b();
void prob2c();
void prob2d();


///////////////////////////////////
/////////// Application ///////////
///////////////////////////////////
void Dithering(int size, int color_number) {
    Halftoning halftoning = Halftoning(512, 512, 1, "p2_image/mandrill.raw");
    halftoning.Apply_Dithering(size, color_number);
    string filename = "p2_result/Dithering_" + to_string(size) + "_" + to_string(color_number) + ".raw";
    halftoning.image.Write(&(halftoning.image), filename);
}

void Error_Diffusion(int mode) {
    Halftoning halftoning = Halftoning(512, 512, 1, "p2_image/mandrill.raw");
    halftoning.Apply_Error_Diffusion(mode);
    string filename = "p2_result/ErrorDiffuse_" + to_string(mode) + ".raw";
    halftoning.image.Write(&(halftoning.image), filename);
}

void Scalar_Color_Halftoning(int mode) {
    Halftoning halftoning = Halftoning(512, 512, 3, "p2_image/Sailboat.raw");
    halftoning.Apply_Scalar_Color_halftoning(mode);
    string filename = "p2_result/ScalarColorHalftoning_" + to_string(mode) + ".raw";
    halftoning.image.Write(&(halftoning.image), filename);
}

void Vector_Color_Halftoning(int mode) {
    Halftoning halftoning = Halftoning(512, 512, 3, "p2_image/Sailboat.raw");
    halftoning.Apply_Vector_Color_Halftoning(mode);
    string filename = "p2_result/VectorColorHalftoning_" + to_string(mode) + ".raw";
    halftoning.image.Write(&(halftoning.image), filename);
}


/////////////////////////////////////////
/////////// Problem Implement ///////////
/////////////////////////////////////////
void prob2a() {
    cout << "Problem 2a" << endl;
    // Intensity: 0, 255
    Dithering(2, 255);
    Dithering(4, 255);
    Dithering(8, 255);
    // Intensity: 0, 85, 170, 255
    Dithering(2, 85);
    Dithering(4, 85);
    Dithering(8, 85);
    Dithering(2, 1);
}

void prob2b() {
    cout << "Problem 2b" << endl;
    Error_Diffusion(0);
    Error_Diffusion(1);
    Error_Diffusion(2);
}

void prob2c() {
    cout << "Problem 2c" << endl;
    Scalar_Color_Halftoning(0);
    Scalar_Color_Halftoning(1);
    Scalar_Color_Halftoning(2);
}

void prob2d() {
    cout << "Problem 2d" << endl;
    Vector_Color_Halftoning(0);
    Vector_Color_Halftoning(1);
    Vector_Color_Halftoning(2);
}


////////////////////////////
/////////// Main ///////////
////////////////////////////
int main() {
    cout << "Homework 3.2" << endl;
    int number = 0;
    switch (number) {
        case 0:
            // Implementation for problem 2
            prob2a();
            prob2b();
            prob2c();
            prob2d();
        case 1:
            prob2a();
            break;
        case 2:
            prob2b();
            break;
        case 3:
            prob2c();
            break;
        case 4:
            prob2d();
            break;
    }
    return 0;
}
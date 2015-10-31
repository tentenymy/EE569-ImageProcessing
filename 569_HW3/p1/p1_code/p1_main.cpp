/* EE569 Homework Assignment #3
 * Date: Nov 1, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * Email: meiyiyan@usc.edu
 *
 * Problem1. Geometrical Modification
 *
 * p1_main.cpp
 * Image.h
 * Image.cpp
 */

#include "Image.h"
using namespace std;

/////////////////////////////////////
/////////// Declaration /////////////
/////////////////////////////////////
// @para degree: max rotation degree
void Swirling(float degree);

// @para degree: max rotation degree
// @para mode:
// MODE_X: rotation on X axis
// MODE_Y: rotation on Y axis
// MODE_Z: rotation on Z axis
void Test_Rotation(float degree, int mode);

// @para tx, ty, tz: translation distances in each X, Y, Z direction
void Test_Translation(float tx, float ty, float tz);

// @para sx, sy, sz: scaling parameters in each X, Y, Z direction
void Test_Scaling(float sx, float sy, float sz);

// @para density: density (number of pixels in unit length)
// @para coef_f: focal length
// @para coord_xc, coord_yc, coord_zc: Xc, Yc, Zc (three vectors of camera coordinates system)
// @para coord_r: r (vector from the origin of the world coordinates to the origin of the camera coordinate)
void Forward_Mapping(int density, float coef_f, Coord3D coord_xc, Coord3D coord_yc, Coord3D coord_zc, Coord3D coord_r);

// @para density: density (number of pixels in unit length)
void Reverse_Mapping(int density);

// @para int mode:
// 0: implement for problem1a, swirling
// 1: test case for rotation
// 2: test case for translation
// 3: test case for scaling
void prob1a(int mode);

// @para int mode:
// 0: implement for problem1b, different density + 3D camera forward mapping
// 1: implement for problem1b, 3D camera reverse mapping
// 2: test case for different f in forward mapping
// 3: test case for different r in forward mapping
void prob1b(int mode);


/////////////////////////////////////
/////////// Application /////////////
/////////////////////////////////////
void Swirling(float degree) {
    Image img_ori(512, 512, 3, "p1_image/kate.raw");
    img_ori.Initial_Geodata();
    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col, 0.0f);
    img_ori.Effect_Swirling(degree);
    img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col, 0.0f);
    img_ori.Set_Data();
    string filename = "Kate_swirling_" + to_string((int)degree) + ".raw";
    img_ori.Write(&img_ori, filename);
}

void Test_Rotation(float degree, int mode) {
    Image img_ori(512, 512, 3, "p1_image/kate.raw");
    img_ori.Initial_Geodata();
    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col, 0.0f);
    img_ori.Effect_Rotation(degree, mode);
    img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col, 0.0f);
    img_ori.Set_Data();
    string filename = "Kate_rotation_" + to_string((int)degree) + ".raw";
    img_ori.Write(&img_ori, filename);

}

void Test_Translation(float tx, float ty, float tz) {
    Image img_ori(512, 512, 3, "p1_image/kate.raw");
    img_ori.Initial_Geodata();
    img_ori.Effect_Translation(tx, ty, tz);
    img_ori.Set_Data();
    string filename = "Kate_translation_" + to_string((int)tx) + "_" + to_string((int)ty) +  "_" + to_string((int)tz) + ".raw";
    img_ori.Write(&img_ori, filename);
}

void Test_Scaling(float sx, float sy, float sz) {
    Image img_ori(512, 512, 3, "p1_image/kate.raw");
    img_ori.Initial_Geodata();
    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col, 0.0f);
    img_ori.Effect_Scaling(1.0f/sx, 1.0f/sy, 1.0f/sz);
    img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col, 0.0f);
    img_ori.Set_Data();
    string filename = "Kate_scaling_" + to_string(sx) + "_" + to_string(sy) +  "_" + to_string(sz) + ".raw";
    img_ori.Write(&img_ori, filename);

}

void Forward_Mapping(int density, float coef_f, Coord3D coord_xc, Coord3D coord_yc, Coord3D coord_zc, Coord3D coord_r) {
    // Read image data
    vector<Image*> image_list;
    Image img_ori0(200, 200, 3, "p1_image/baby.raw");
    img_ori0.Initial_Geodata();
    img_ori0.Effect_Translation(-100.0f, -100.0f, 1.0f);
    img_ori0.Effect_Rotation(90.0f, MODE_Z);
    img_ori0.Effect_Scaling(0.01, 0.01, 1.0f);

    Image img_ori1(200, 200, 3, "p1_image/baby_bear.raw");
    img_ori1.Initial_Geodata();
    img_ori1.Effect_Translation(-100, -100, 1);
    img_ori1.Effect_Rotation(90, MODE_Y);
    img_ori1.Effect_Rotation(90, MODE_X);
    img_ori1.Effect_Scaling(1.0f, 0.01, 0.01);

    Image img_ori2(200, 200, 3, "p1_image/baby_cat.raw");
    img_ori2.Initial_Geodata();
    img_ori2.Effect_Translation(-100, -100, 1);
    img_ori2.Effect_Rotation(-90, MODE_X);
    img_ori2.Effect_Rotation(180, MODE_Y);
    img_ori2.Effect_Scaling(0.01, 1.0f, 0.01);

    image_list.push_back(&img_ori0);
    image_list.push_back(&img_ori1);
    image_list.push_back(&img_ori2);

    // Set to new image
    Image img(200, 200, 3, (ImgPixel *)NULL);
    img.Initial_Geodata(image_list);

    // Extrinsic Camera
    img.Camera_Extrinsic(coord_xc, coord_yc, coord_zc, coord_r);

    // Intrinsic Camera
    img.Camera_Intrinsic(coef_f, 2.0f, 2.0f);

    // Display
    img.Set_Camera_Data(density);
    string filename = "cube" + to_string(density) + "_r" + to_string(coord_r[0]) + ".raw";
    img.Write(&img, filename);
}

void Reverse_Mapping(int density) {
    // Read image data
    vector<Image*> image_list;

    Image img_ori0(200, 200, 3, "p1_image/baby.raw");
    img_ori0.Initial_Geodata();
    img_ori0.Effect_Translation(-100.0f, -100.0f, 1.0f);
    img_ori0.Effect_Rotation(90.0f, MODE_Z);
    img_ori0.Effect_Scaling(0.01, 0.01, 1.0f);

    Image img_ori1(200, 200, 3, "p1_image/baby_bear.raw");
    img_ori1.Initial_Geodata();
    img_ori1.Effect_Translation(-100, -100, 1);
    img_ori1.Effect_Rotation(90, MODE_Y);
    img_ori1.Effect_Rotation(90, MODE_X);
    img_ori1.Effect_Scaling(1.0f, 0.01, 0.01);

    Image img_ori2(200, 200, 3, "p1_image/baby_cat.raw");
    img_ori2.Initial_Geodata();
    img_ori2.Effect_Translation(-100, -100, 1);
    img_ori2.Effect_Rotation(-90, MODE_X);
    img_ori2.Effect_Rotation(180, MODE_Y);
    img_ori2.Effect_Scaling(0.01, 1.0f, 0.01);

    image_list.push_back(&img_ori0);
    image_list.push_back(&img_ori1);
    image_list.push_back(&img_ori2);

    // Set to new image
    Image img(200, 200, 3, (ImgPixel *)NULL);
    img.Initial_Geodata(image_list);

    // Extrinsic Camera
    Coord3D coord_xc = {-1.0f, 1.0f, 0.0f};
    Coord3D coord_yc = {1.0f, 1.0f, -2.0f};
    Coord3D coord_zc = {-1.0f, -1.0f, -1.0f};
    Coord3D coord_r = {5.0f, 5.0f, 5.0f};
    img.Camera_Extrinsic(coord_xc, coord_yc, coord_zc, coord_r);

    // Intrinsic Camera
    float coef_f = (float)sqrt(3.0);
    img.Camera_Intrinsic(coef_f, 2.0f, 2.0f);

    // Reverse Mapping
    Image img_reverse(200, 200, 3, (ImgPixel*) NULL);
    img_reverse.Initial_Geodata(img.geo_data, img.geo_row, img.geo_col);

    // Reverse Intrinsic Camera
    img_reverse.Camera_Intrinsic_Reverse(coef_f, 2.0f, 2.0f);

    // Reverse Extrinsic Camera
    img_reverse.Camera_Extrinsic_Reverse(coord_xc, coord_yc, coord_zc, coord_r);
    img_reverse.Print_Geodata_Coord("Extrinsic", 600, 4);
    img_reverse.Set_Camera_Data_Reverse();
    img_reverse.Write(&img_reverse, "test0.raw");
}


////////////////////////////////////////////////
/////////// Problem implementation /////////////
////////////////////////////////////////////////
void prob1a (int mode) {
    cout << "Problem 1a" << endl;
    switch (mode) {
        case 0:
            Swirling(-90.0f);
            Swirling(-45.0f);
            Swirling(45.0f);
            Swirling(90.0f);
            Swirling(180.0f);
            break;
        case 1:
            Test_Rotation(90, MODE_Z);
            Test_Rotation(60, MODE_Z);
            Test_Rotation(30, MODE_Z);
            break;
        case 2:
            Test_Translation(256, 256, 0);
            Test_Translation(256, 0, 0);
            Test_Translation(-256, 0, 0);
            Test_Translation(-256, -256, 0);
            Test_Translation(512, 0, 0);
            break;
        case 3:
            Test_Scaling(1.5f, 1.0f, 1.0f);
            Test_Scaling(1.0f, 1.5f, 1.0f);
            Test_Scaling(1.0f, 1.0f, 1.5);
            Test_Scaling(0.5f, 1.0f, 1.0f);
            Test_Scaling(1.0f, 0.5f, 1.0f);
            break;
    }
}

void prob1b(int mode) {
    cout << "Problem 1b" << endl;
    Coord3D coord_xc = {-1.0f, 1.0f, 0.0f};
    Coord3D coord_yc = {1.0f, 1.0f, -2.0f};
    Coord3D coord_zc = {-1.0f, -1.0f, -1.0f};
    Coord3D coord_r = {5.0f, 5.0f, 5.0f};
    float coef_f = (float)sqrt(3.0);

    switch(mode) {
        case 0:
            Forward_Mapping(300, coef_f, coord_xc, coord_yc, coord_zc, coord_r);
            Forward_Mapping(250, coef_f, coord_xc, coord_yc, coord_zc, coord_r);
            Forward_Mapping(200, coef_f, coord_xc, coord_yc, coord_zc, coord_r);
            Forward_Mapping(150, coef_f, coord_xc, coord_yc, coord_zc, coord_r);
            Forward_Mapping(100, coef_f, coord_xc, coord_yc, coord_zc, coord_r);
            Forward_Mapping(50, coef_f, coord_xc, coord_yc, coord_zc, coord_r);
            break;
        case 1:
            Reverse_Mapping(100);
            break;
        case 2:
            Forward_Mapping(200, 0.0, coord_xc, coord_yc, coord_zc, coord_r);
            Forward_Mapping(200, 1.0, coord_xc, coord_yc, coord_zc, coord_r);
            Forward_Mapping(200, 2.0, coord_xc, coord_yc, coord_zc, coord_r);
            break;
        case 3:
            Coord3D coord_r0 = {-3.0f, -3.0f, -3.0f};
            Coord3D coord_r1 = {0.0f, 0.0f, 0.0f};
            Coord3D coord_r2 = {3.0f, 3.0f, 3.0f};
            Coord3D coord_r3 = {6.0f, 6.0f, 6.0f};
            Coord3D coord_r4 = {12.0f, 12.0f, 12.0f};
            Forward_Mapping(200, coef_f, coord_xc, coord_yc, coord_zc, coord_r0);
            Forward_Mapping(200, coef_f, coord_xc, coord_yc, coord_zc, coord_r1);
            Forward_Mapping(200, coef_f, coord_xc, coord_yc, coord_zc, coord_r2);
            Forward_Mapping(200, coef_f, coord_xc, coord_yc, coord_zc, coord_r3);
            Forward_Mapping(200, coef_f, coord_xc, coord_yc, coord_zc, coord_r4);
            break;
    }
}


///////////////////////////////////////
/////////// Main function /////////////
///////////////////////////////////////
int main() {
    cout << "Hello, World!" << endl;
    int mode = 0;
    switch (mode) {
        case 0:
            // Implementation for problem 1
            prob1a(0);
            prob1b(0);
            prob1b(1);
            break;
        case 1:
            // Test
            prob1a(0);
            prob1a(1);
            prob1a(2);
            prob1a(3);

            prob1b(0);
            prob1b(1);
            prob1b(2);
            prob1b(3);
            break;
    }
    return 0;
}
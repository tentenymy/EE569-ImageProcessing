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

#ifndef P1_IMAGE_H
#define P1_IMAGE_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;


//////////////////////////////////////////////
/////////// Structure Definition /////////////
//////////////////////////////////////////////
const double PI = 3.14159265;

const int MODE_X = 0;
const int MODE_Y = 1;
const int MODE_Z = 2;

typedef unsigned char ImgPixel;
typedef float ImgCoord[4];
typedef float ImgColor[3];
typedef float Matrix[4][4];
typedef float Coord3D[3];

typedef struct GeoPixel {
    ImgCoord coord;
    ImgColor color;
} GeoPixel;


//////////////////////////////////////////////
/////////// Functions Definition /////////////
//////////////////////////////////////////////
float COS(float);
float SIN(float);
float TAN(float);


//////////////////////////////////////////
/////////// Class Definition /////////////
//////////////////////////////////////////
class Image {
public:
    int col;
    int row;
    int byte;
    int geo_row;
    int geo_col;
    ImgPixel *data = NULL;
    GeoPixel *geo_data = NULL;

/////////////////////////////////////
/////////// Constructor /////////////
/////////////////////////////////////
// @para new_row: image row
// @para new_col: image col
// @para new_byte: image byte (1 or 3)
// @para pt_img: image data buffer (row * col * byte size)
    Image(int new_row, int new_col, int new_byte, ImgPixel *pt_img);

// @para new_row: image row
// @para new_col: image col
// @para new_byte: image byte (1 or 3)
// @para filename: filename of image
    Image(int new_row, int new_col, int new_byte, string filename);

    ~Image();


/////////////////////////////////
/////////// Display /////////////
/////////////////////////////////
// @para image: image want to write
// @para filename: filename of image to write to
    void Write (Image *image, string filename);

// @para str: comment to print
// @para new_row: max row to print
// @para new_col: max col to print
    void Print_Data(string str);
    void Print_Geodata_Color(string str);
    void Print_Geodata_Color(string str, int new_row, int new_col);
    void Print_Geodata_Coord(string str);
    void Print_Geodata_Coord(string str, int new_row, int new_col);


/////////////////////////////////////
/////////// Access Data /////////////
/////////////////////////////////////
// @para i, j, k: row, column, byte number index to access
// @para index: index want to access
    ImgPixel Get_Value(int i, int j, int k);
    float Get_Geo_Coord(int i, int j, int k);
    float Get_Geo_Value(int i, int j, int k);
    int Get_Row(int index);
    int Get_Col(int index);
    int Get_Byte(int index);


//////////////////////////////////
/////////// Geometry /////////////
//////////////////////////////////
// @para image_list: a list of images
// @para new_geo_data: copy a geo_data to this class
// @para new_geo_row, new_geo_col: set the row, column of geometry data
// @para matrix: matrix to multiply to the geodata matrix;
    int Initial_Geodata();
    int Initial_Geodata(vector <Image*> image_list);
    int Initial_Geodata(GeoPixel* new_geo_data, int new_geo_row, int new_geo_col);
    int Image_to_Cartesian_Coordinate();
    int Cartesian_to_Image_Coordinate();
    int Apply_Matrix(Matrix matrix);
    int Set_Data();


////////////////////////////////
/////////// Effect /////////////
////////////////////////////////
// @para theta: max rotation degree
// @para mode:
// MODE_X: rotation on X axis
// MODE_Y: rotation on Y axis
// MODE_Z: rotation on Z axis
    int Effect_Rotation(float theta, int mode);

// @para tx, ty, tz: translation distances in each X, Y, Z direction
    int Effect_Translation(float coef_tx, float coef_ty, float coef_tz);

// @para sx, sy, sz: scaling parameters in each X, Y, Z direction
    int Effect_Scaling(float coef_sx, float coef_sy, float coef_sz);

// @para degree: max rotation degree
    int Effect_Swirling(float theta);


///////////////////////////////////
/////////// 3D camera /////////////
///////////////////////////////////
// @para density: density (number of pixels in unit length)
// @para coord_xc, coord_yc, coord_zc: Xc, Yc, Zc (three vectors of camera coordinates system)
// @para coord_r: r (vector from the origin of the world coordinates to the origin of the camera coordinate)
// @para coef_f: focal length
// @para coef_cx, coef_cy: iamge coordinates of the intersecting point between the optical axis and the image plane (2.0f, 2.0f in this case)
    int Camera_Extrinsic(Coord3D coord_xc, Coord3D coord_yc, Coord3D coord_zc, Coord3D coord_r);
    int Camera_Intrinsic(float coef_f, float coef_cx, float coef_cy);
    int Set_Camera_Data(int density);
    int Camera_Intrinsic_Reverse(float coef_f, float coef_cx, float coef_cy);
    int Camera_Extrinsic_Reverse(Coord3D coord_xc, Coord3D coord_yc, Coord3D coord_zc, Coord3D coord_r);
    int Set_Camera_Data_Reverse();
};
#endif //P1_IMAGE_H

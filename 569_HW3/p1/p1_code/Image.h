//
// Created by Maggie on 10/23/15.
//

#ifndef P1_IMAGE_H
#define P1_IMAGE_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <cstring>

using namespace std;


const double PI = 3.14159265;

const int X = 0;
const int Y = 1;
const int Z = 2;

typedef unsigned char ImgPixel;
typedef float ImgCoord[4];
typedef float ImgColor[3];

typedef float Matrix[4][4];


typedef struct GeoPixel {
    ImgCoord coord;
    ImgColor color;
} GeoPixel;


float COS(float);
float SIN(float);
float TAN(float);



class Image {
public:
    int col;
    int row;
    int byte;
    ImgPixel *data = NULL;
    GeoPixel *geo_data = NULL;

    // Constructor
    Image(int new_row, int new_col, int new_byte, ImgPixel *pt_img);
    Image(int new_row, int new_col, int new_byte, string filename);
    ~Image();

    // Display
    void Write (Image *image, string filename);

    void Print_Data(string);
    void Print_Geodata_Color(string);
    void Print_Geodata_Coord(string);
    void Print_Geodata_Coord(string, int, int);

    ImgPixel Get_Value(int i, int j, int k);
    int Get_Row(int index);
    int Get_Col(int index);
    int Get_Byte(int index);

    // Geometry
    int Initial_Geodata();
    int Image_to_Cartesian_Coordinate();
    int Cartesian_to_Image_Coordinate();

    int Apply_Matrix(Matrix);
    int Set_Data();


    // Effect
    int Effect_Rotation(float, int);
    int Effect_Translation(float, float, float);
    int Effect_Scaling(float, float, float);
    int Effect_Swirling(float);


    // Camera




};
#endif //P1_IMAGE_H

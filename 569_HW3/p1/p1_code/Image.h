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

typedef unsigned char ImgPixel;
typedef float ImgCoord[3];
typedef float ImgColor[3];

typedef float Matrix[3][3];

typedef struct GeoPixel {
    ImgCoord coord;
    ImgColor color;
} GeoPixel;


class Image {
public:
    int col;
    int row;
    int byte;
    ImgPixel *data = NULL;
    GeoPixel *geo_data = NULL;


    Image(int new_row, int new_col, int new_byte, ImgPixel *pt_img);
    Image(int new_row, int new_col, int new_byte, string filename);
    ~Image();

    void Write (Image *image, string filename);
    void Print_Data();
    void Print_Geodata_Color();
    void Print_Geodata_Coord();

    int Initial_Geodata();
    int Convert_Cartesian_Coordinate();
    int Convert_Image_Coordinate();

    ImgPixel Get_Value(int i, int j, int k);
    int Get_Row(int index);
    int Get_Col(int index);
    int Get_Byte(int index);
};




#endif //P1_IMAGE_H

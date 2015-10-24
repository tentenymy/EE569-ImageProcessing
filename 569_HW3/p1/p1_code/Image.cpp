//
// Created by Maggie on 10/23/15.
//

#include "Image.h"



///////////////////////////////
/////////// image /////////////
///////////////////////////////

Image::Image(int new_row, int new_col, int new_byte, ImgPixel *pt_img) {
    if (new_col <= 0 || new_col > 1024) {
        cerr << "wrong column" << endl;
        exit(1);
    }
    if (new_row <= 0 || new_row > 1024) {
        cerr << "wrong column" << endl;
        exit(1);
    }
    col = new_col;
    row = new_row;
    byte = new_byte;
    data = new ImgPixel [col * row * byte];
    if (!data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }
    for (int i = 0; i < col * row * byte; i++) {
        data[i] = *pt_img++;
    }
}

Image::Image(int new_row, int new_col, int new_byte, string filename) {
    if (new_col <= 0 || new_col > 1024) {
        cerr << "Wrong column" << endl;
        exit(1);
    }
    if (new_row <= 0 || new_row > 1024) {
        cerr << "Wrong row" << endl;
        exit(1);
    }
    col = new_col;
    row = new_row;
    byte = new_byte;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    FILE *file;
    if (!(file = fopen(c_filename, "rb")))  {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    data = new ImgPixel[col * row * byte];
    if (!data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }
    fread(data, sizeof(ImgPixel), col * row * byte, file);
    fclose(file);
    if (c_filename)
        delete c_filename;
}

Image::~Image() {
    if(data)
        delete data;
    if (geo_data)
        delete geo_data;
}

void Image::Write (Image *image, string filename) {
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    FILE *file;
    if (!(file = fopen(c_filename, "wb"))) {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = image->col * image->row * image->byte;
    fwrite(image->data, sizeof(ImgPixel), col * row * byte, file);
    fclose(file);
}

void Image::Print() {
    cout << endl << "Image: " << row << ", " << col << ", " << byte << endl;
    int count = 0;
    int new_col = col;
    int max_col = 100;
    if (col * byte > max_col) {
        new_col = max_col / byte;
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < new_col; j++) {
            for (int k = 0; k < byte; k++) {
                cout << (int)Get_Value(i, j, k) << " ";
            }
        }
        cout << endl;
    }
}

ImgPixel Image::Get_Value(int i, int j, int k) {
    return data[i * byte * col + j * byte + k];
}

int Image::Get_Row(int index) {
    return index / (col * byte);
}
int Image::Get_Col(int index) {
    return (index % (byte * col)) / byte;
}
int Image::Get_Byte(int index) {
    return index % byte;
}


void Image::Get_Image_Coordinate() {
    geo_data = new GeoPixel[row * col];
    if (!geo_data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            geo_data[i * col + j];
        }
    }





}
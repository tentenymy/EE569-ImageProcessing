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

void Image::Print_Data() {
    cout << endl << "Image: " << row << ", " << col << ", " << byte << endl;
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

void Image::Print_Geodata_Color() {
    cout << endl << "Image: " << row << ", " << col << ", " << byte << endl;
    if (geo_data) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << "(" << geo_data[i * col + j].coord[0] << " "
                << geo_data[i * col + j].coord[1] << " "
                << geo_data[i * col + j].coord[2] << " "
                << geo_data[i * col + j].color[0] << " "
                << geo_data[i * col + j].color[1] << " "
                << geo_data[i * col + j].color[2] << ") ";
            }
            cout << endl;
        }
    }
}

void Image::Print_Geodata_Coord() {
    cout << endl << "Image: " << row << ", " << col << ", " << byte << endl;
    if (geo_data) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << "(" << geo_data[i * col + j].coord[0] << " "
                << geo_data[i * col + j].coord[1] << " "
                << geo_data[i * col + j].coord[2] << ") ";
            }
            cout << endl;
        }
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

int Image::Initial_Geodata() {
    // allocate memory to geo_data
    if (geo_data) {
        cerr << "geo data already exist" << endl;
        return 0;
    }
    geo_data = new GeoPixel[row * col];
    if (!geo_data) {
        cerr << "Wrong allocate memory" << endl;
        return 0;
    }

    // image coordinate
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            geo_data[i * col + j].coord[0] = i;
            geo_data[i * col + j].coord[1] = j;
            geo_data[i * col + j].coord[2] = 1;
            geo_data[i * col + j].color[0] = Get_Value(i, j, 0);
            geo_data[i * col + j].color[1] = Get_Value(i, j, 1);
            geo_data[i * col + j].color[2] = Get_Value(i, j, 2);
        }
    }
    return 1;
}


// Convert image data to Cartesian Coordinate
int Image::Convert_Cartesian_Coordinate() {
    // check geo_data
    if (!geo_data) {
        cerr << "geo data is still null" << endl;
        return 0;
    }

    // Initial matrix
    float temp_row = (float)row - 0.5f;
    Matrix matrix = {{0.0f, 1.0f, 0.5f}, {-1.0f, 0.0f, temp_row}, {0.0f, 0.0f, 1.0f}};

    // apply matrix to Image coordinate
    ImgCoord temp_coord = {};
    for(int i = 0; i < row * col; i++) {
        for (int j = 0; j < 3; j++) {
            temp_coord[j] = 0;
            for (int k = 0; k < 3; k++) {
                temp_coord[j] += matrix[j][k] * geo_data[i].coord[k];
            }
        }
        geo_data[i].coord[0] = temp_coord[0];
        geo_data[i].coord[1] = temp_coord[1];
        geo_data[i].coord[2] = temp_coord[2];
    }
    return 1;
}

// Convert Cartesian Coordiante to Image Coordiante
int Image::Convert_Image_Coordinate() {
    // check geo_data
    if (!geo_data) {
        cerr << "geo data is still null" << endl;
        return 0;
    }

    // Initial matrix
    float temp_row = (float)row - 0.5f;
    Matrix matrix = {{0.0f, 1.0f, 0.5f}, {-1.0f, 0.0f, temp_row}, {0.0f, 0.0f, 1.0f}};

    // apply matrix to Image coordinate
    ImgCoord temp_coord = {};
    for(int i = 0; i < row * col; i++) {
        for (int j = 0; j < 3; j++) {
            temp_coord[j] = 0;
            for (int k = 0; k < 3; k++) {
                temp_coord[j] += matrix[j][k] * geo_data[i].coord[k];
            }
        }
        geo_data[i].coord[0] = temp_coord[0];
        geo_data[i].coord[1] = temp_coord[1];
        geo_data[i].coord[2] = temp_coord[2];
    }
    return 1;
}
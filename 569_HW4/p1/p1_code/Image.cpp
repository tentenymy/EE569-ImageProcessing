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

#include "Image.h"

float COS(float degree) {
    return cos(degree * PI / 180.0);
}

float SIN(float degree) {
    return sin(degree * PI / 180.0);
}

float TAN (float degree) {
    return tan(degree * PI / 180.0);
}

/////////////////////////////////////
/////////// Constructor /////////////
/////////////////////////////////////
Image::Image() {
    col = 0;
    row = 0;
    byte = 0;
}

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
    if (pt_img == NULL) {
        for (int i = 0; i < col * row * byte; i++) {
            data[i] = 0;
        }

    } else {
        for (int i = 0; i < col * row * byte; i++) {
            data[i] = *pt_img++;
        }
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


/////////////////////////////////
/////////// Display /////////////
/////////////////////////////////
void Image::Write (Image *image, string filename) {
    cout << "Write " << filename << endl;
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

void Image::Write (string filename) {
    //cout << "Write " << filename << endl;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    FILE *file;
    if (!(file = fopen(c_filename, "wb"))) {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = col * row * byte;
    fwrite(data, sizeof(ImgPixel), col * row * byte, file);
    fclose(file);
}

void Image::Write_Txt (string filename) {
    cout << "Write " << filename << endl;
    fstream fout;
    fout.open(filename, ios_base::out);
    if (!fout.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            for (int k = 0; k < byte; k++)
                fout << (int)data[i * col * byte + j * byte + k]<< " ";
        }
        fout << endl;
    }
    fout.close();
}


void Image::Write (string filename, int mode) {
    //cout << "Write " << filename << endl;
    ofstream fout;
    fout.open(filename);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            for (int k = 0; k < byte; k++) {
                if ((int)(data[i * byte * col + j * byte + k] == 255))
                    fout << "1 ";
                else
                    fout << "  ";
            }
        }
        fout << endl;
    }
    fout.close();
}

void Image::Print_Data(string str) {
    cout << endl << "Image " << str << ": " << row << ", " << col << ", " << byte << endl;
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

void Image::Print_Pattern_Data(string str) {
    int new_row = 256;
    int new_col = 100;
    if (row < new_row)
        new_row = row;
    if (col < new_col)
        new_col = col;
    cout << endl << "Image " << str << ": " << row << ", " << col << ", " << byte << endl;
    for (int i = 0; i < new_row; i++) {
        cout << "ROW " << i << ": ";
        for (int j = 0; j < new_col; j++) {
            for (int k = 0; k < byte; k++) {
                if (Get_Value(i, j, k) == 0)
                    cout << " ";
                else
                    cout << (int)Get_Value(i, j, k);
            }
        }
        cout << endl;
    }
}

void Image::Print_Data(string str, int new_row, int new_col) {
    cout << endl << "Image " << str << ": " << row << ", " << col << ", " << byte << endl;
    if (new_row > row)
        new_row = row;
    if (new_col > col)
        new_col = col;
    for (int i = 0; i < new_row; i++) {
        for (int j = 0; j < new_col; j++) {
            for (int k = 0; k < byte; k++) {
                cout << (int)Get_Value(i, j, k) << " ";
            }
        }
        cout << endl;
    }
}

void Image::Print_Geodata_Color(string str) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (geo_data) {
        for (int i = 0; i < geo_row; i++) {
            for (int j = 0; j < geo_col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << " "
                << geo_data[i * geo_col + j].color[0] << " "
                << geo_data[i * geo_col + j].color[1] << " "
                << geo_data[i * geo_col + j].color[2] << ") ";
            }
            cout << endl;
        }
    }
}

void Image::Print_Geodata_Color(string str, int new_row, int new_col) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (new_row > geo_row)
        new_row = geo_row;
    if (new_col > geo_col)
        new_col = geo_col;
    if (geo_data) {
        for (int i = 0; i < new_row; i++) {
            for (int j = 0; j < new_col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << " "
                << geo_data[i * geo_col + j].color[0] << " "
                << geo_data[i * geo_col + j].color[1] << " "
                << geo_data[i * geo_col + j].color[2] << ") ";
            }
            cout << endl;
        }
    }
}

void Image::Print_Geodata_Coord(string str) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (geo_data) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << ") ";
            }
            cout << endl;
        }
    }
}

void Image::Print_Geodata_Coord(string str, int new_row, int new_col) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (new_row > geo_row)
        new_row = geo_row;
    if (new_col > geo_col)
        new_col = geo_col;
    if (geo_data) {
        for (int i = 0; i < new_row; i++) {
            for (int j = 0; j < new_col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << ") ";
            }
            cout << endl;
        }
    }
}


/////////////////////////////////////
/////////// Access Data /////////////
/////////////////////////////////////
ImgPixel Image::Get_Value(int i, int j, int k) {
    if (i < 0 || i >= row || j < 0 || j >= col || k < 0 || k >= byte)
        return 0;
    return data[i * byte * col + j * byte + k];
}

ImgPixel* Image::Get_Pixel(int i, int j, int k) {
    return &data[i * byte * col + j * byte + k];
}

float Image::Get_Geo_Value(int i, int j, int k) {
    if (i < geo_row && j < geo_col && k < 3)
        return geo_data[i * geo_col + j].color[k];
    else
        return (ImgPixel)0;
}

float Image::Get_Geo_Coord(int i, int j, int k) {
    if (i < geo_row && j < geo_col && k < 4)
        return geo_data[i * geo_col + j].coord[k];
    else
        return (ImgPixel)0;
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


/////////////////////////////////
/////////// Process /////////////
/////////////////////////////////


void Image::Convert_To_Binary() {
    if (byte == 1) {
        for (int i = 0; i < row * col; i++) {
            ImgPixel temp = data[i];
            if (temp > (ImgPixel)170) {
                data[i] = (ImgPixel)1;
            } else {
                data[i] = (ImgPixel)0;
            }
        }
    }
}

void Image::Convert_Size(int new_row, int new_col) {
    double ratio_row = (double)(row - 1) / (double)(new_row - 1);
    double ratio_col = (double)(col - 1) / (double)(new_col - 1);
    double point_estimate_x;
    double point_estimate_y;
    int point_lt_x;
    int point_lt_y;
    double scale_a;
    double scale_b;
    int point_rb_x;
    int point_rb_y;

    ImgPixel *data_new = new ImgPixel[new_row * new_col * byte];
    for(int i = 0; i < new_row; i++)  {
        for(int j = 0; j < new_col; j++) {
            point_estimate_x = ratio_row * (double)i;
            point_estimate_y = ratio_col *  (double)j;
            point_lt_x = (int)point_estimate_x;
            point_lt_y = (int)point_estimate_y;
            scale_a = point_estimate_x - (double)point_lt_x;
            scale_b = point_estimate_y - (double)point_lt_y;
            for(int k = 0; k < byte; k++) {
                double value = (1.0 - scale_a) * (1.0 - scale_b) * (double)Get_Value(point_lt_x, point_lt_y, k)
                               + (1.0 - scale_a) * scale_b * (double)Get_Value(point_lt_x, point_lt_y + 1, k)
                               + (1.0 - scale_b) * scale_a * (double)Get_Value(point_lt_x + 1, point_lt_y, k)
                               + scale_a * scale_b * (double)Get_Value(point_lt_x + 1, point_lt_y + 1, k);
                data_new[i * (new_col) * byte + j * byte + k] = (ImgPixel)round(value);
            }
        }
    }

    delete data;
    row = new_row;
    col = new_col;
    data = new ImgPixel[row * col * byte];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            for (int k = 0; k < byte; k++)
                data[i * col * byte + j * byte + k] = data_new[i * col * byte + j * byte + k];
        }
    }
}


void Image::Convert_Row_Size(int new_row) {
    double ratio_row = (double)(row - 1) / (double)(new_row - 1);
    double point_estimate_x;
    double point_estimate_y;
    int point_lt_x;
    int point_lt_y;
    double scale_a;
    double scale_b;
    int point_rb_x;
    int point_rb_y;

    ImgPixel *data_new = new ImgPixel[new_row * col * byte];
    for(int i = 0; i < new_row; i++)  {
        for(int j = 0; j < col; j++) {
            point_estimate_x = ratio_row * (double)i;
            point_lt_x = (int)point_estimate_x;
            point_lt_y = (int)point_estimate_y;
            scale_a = point_estimate_x - (double)point_lt_x;
            scale_b = point_estimate_y - (double)point_lt_y;
            for(int k = 0; k < byte; k++) {
                double value = (1.0 - scale_a) * (1.0 - scale_b) * (double)Get_Value(point_lt_x, point_lt_y, k)
                               + (1.0 - scale_a) * scale_b * (double)Get_Value(point_lt_x, point_lt_y + 1, k)
                               + (1.0 - scale_b) * scale_a * (double)Get_Value(point_lt_x + 1, point_lt_y, k)
                               + scale_a * scale_b * (double)Get_Value(point_lt_x + 1, point_lt_y + 1, k);
                data_new[i * col * byte + j * byte + k] = (ImgPixel)round(value);
            }
        }
    }

    delete data;
    row = new_row;
    data = new ImgPixel[row * col * byte];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            for (int k = 0; k < byte; k++)
                data[i * col * byte + j * byte + k] = data_new[i * col * byte + j * byte + k];
        }
    }
}
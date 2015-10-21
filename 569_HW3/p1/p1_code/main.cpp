#include <iostream>
#include "hw2_helper.h"
#include "math.h"

const float PI = 3.1415926f;
const float DEGREE = 180.0f;

using namespace std;

float COS(float degree) {
    return cos(degree / DEGREE * PI);
}
float SIN(float degree) {
    return sin(degree / DEGREE * PI);
}
float TAN (float degree) {
    return tan(degree / DEGREE * PI);
}

typedef unsigned char PixelRGB[3];
typedef unsigned char Pixel;

class Matrix {
public:
    int col;
    int row;
    float *data;
    Matrix(int new_row, int new_col, float *pt_data) {
        if (new_row <= 0 || new_col <= 0 || pt_data == NULL) {
            cerr << "Wrong matrix input" << endl;
        }
        col = new_col;
        row = new_row;
        data = new float[col * row];
        for (int i = 0; i < col * row; i++) {
            data[i] = *pt_data++;
        }
    }

    Matrix Multiply(Matrix mat1, Matrix mat2) {

        if (mat1.col != mat2.row) {
            cout << "cannot not multiply" << endl;
            exit(1);
        } else {
            float data_new[mat1.row * mat2.col];
            for (int i = 0; i < mat1.row; i++) {
                for (int j = 0; j < mat2.col; j++) {
                    data_new[i * mat2.col + j] = 0;
                    for (int k = 0; k < mat1.col; k++) {
                        data_new[i * mat2.col + j] += mat1.Get_Pixel(i, k) * mat2.Get_Pixel(k, j);
                    }
                }
            }
            return Matrix(mat1.row, mat2.col, data_new);
        }

    }

    void Print() {
        cout << endl << "Matrix: " << row << ", " << col  << endl;
        int count = 0;
        int new_col = col;
        int max_col = 100;
        if (col > max_col) {
            new_col = max_col;
        }
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < new_col; j++) {
                    cout << Get_Pixel(i, j) << " ";
            }
            cout << endl;
        }
    }

    float Get_Pixel(int i, int j) {
        return data[i * col + j];
    }
};


class Image {
public:
    int col;
    int row;
    int byte;
    Pixel *data;

public:
    Image(int new_row, int new_col, int new_byte, Pixel *pt_img) {
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
        data = new Pixel [col * row * byte];
        for (int i = 0; i < col * row * byte; i++) {
            data[i] = *pt_img++;
        }
    }

    Image(int new_row, int new_col, int new_byte, string filename) {
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
        data = new Pixel[col * row * byte];
        fread(data, sizeof(Pixel), col * row * byte, file);
        fclose(file);
    }

    void Write (Image *image, string filename) {
        char *c_filename = new char[filename.length() + 1];
        strcpy(c_filename, filename.c_str());
        FILE *file;
        if (!(file = fopen(c_filename, "wb"))) {
            cout << "Cannot open file: " << filename << endl;
            exit(1);
        }
        size_t size = image->col * image->row * image->byte;
        fwrite(image->data, sizeof(Pixel), col * row * byte, file);
        fclose(file);
    }

    void Print() {
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
                    cout << (int)Get_Pixel(i, j, k) << " ";
                }
            }
            cout << endl;
        }
    }

    Pixel Get_Pixel(int i, int j, int k) {
        return data[i * byte * col + j * byte + k];
    }

    int Get_Row(int index) {
        return index / (col * byte);
    }
    int Get_Col(int index) {
        return (index % (byte * col)) / byte;
    }
    int Get_Byte(int index) {
        return index % byte;
    }


};

void Test() {
    Pixel img[18] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    Image img_ori(3, 6, 1, &img[0]);
    img_ori.Print();
    //Image img2 (512, 512, 3, "p1_image/lenna.raw");
    //img2.Print();
    //img2.Write(&img2, "test.raw");
    int count = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 6; j++) {
            cout << img_ori.Get_Row(count++) << " ";
        }
        cout << endl;
    }

    float data[6] = {1, 2, 0, 0, 0, 0};
    Matrix mat (6, 1, data);
    mat.Print();

    float img2[18] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    Matrix mat2 (3, 6, img2);
    mat2.Print();

    Matrix mat_new = mat.Multiply(mat2, mat);
    mat_new.Print();


}

void prob1a () {

    Test();

}




int main() {
    cout << "Hello, World!" << endl;
    prob1a();
    return 0;
}
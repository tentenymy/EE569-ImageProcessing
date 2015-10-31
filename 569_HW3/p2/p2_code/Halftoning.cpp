//
// Created by Maggie on 10/30/15.
//

#include "Halftoning.h"

////////////////////////////////////////////
/////////////// Constructors ///////////////
////////////////////////////////////////////
Halftoning::Halftoning(int new_row, int new_col, int new_byte, string filename) {
    Image new_image = Image(new_row, new_col, new_byte, filename);
    image.col = new_image.col;
    image.row = new_image.row;
    image.byte = new_image.byte;
    image.data = new ImgPixel[image.col * image.row * image.byte];
    if (!image.data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }
    for (int i = 0; i < image.col * image.row * image.byte; i++) {
        image.data[i] = new_image.data[i];
    }
}

Halftoning::Halftoning(Image *new_image) {
        image.col = new_image->col;
        image.row = new_image->row;
        image.byte = new_image->byte;
        image.data = new ImgPixel[image.col * image.row * image.byte];
        if (!image.data) {
            cerr << "Wrong allocate memory" << endl;
            exit(1);
        }
        for (int i = 0; i < image.col * image.row * image.byte; i++) {
            image.data[i] = new_image->data[i];
        }
    }

Halftoning::~Halftoning() { }


//////////////////////////////////////////////////////////
/////////////// Grayscale Image Halftoning ///////////////
//////////////////////////////////////////////////////////
int Halftoning::Apply_Dithering(int size, int color_number) {
    // Check grayscale iamge
    if (image.byte != 1) {
        cerr << "Image must be grayscale" << endl;
        return 0;
    }

    // Set Dithering Matrix
    float *bayer_matrix = Generate_Bayer_Matrix(size, color_number);
    int bayer_matrix_row = 0;
    int bayer_matrix_col = 0;
    int color_size = 255 / color_number;
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            int value = (int)((int)(*image.Get_Pixel(i, j, 0)) / bayer_matrix[bayer_matrix_row * size + bayer_matrix_col]);
            if (value > color_size)
                value = color_size;
            *image.Get_Pixel(i, j, 0) = (ImgPixel)(color_number * value);
            bayer_matrix_col++;
            if (bayer_matrix_col >= size)
                bayer_matrix_col = 0;
        }
        bayer_matrix_col = 0;
        bayer_matrix_row++;
        if (bayer_matrix_row >= size)
            bayer_matrix_row = 0;
    }
    delete bayer_matrix;
    return 1;
}

int Halftoning::Apply_Error_Diffusion(int mode) {
    // Check grayscale iamge
    if (image.byte != 1) {
        cerr << "Image must be grayscale" << endl;
        return 0;
    }

    // Set Error Diffussion matrix
    int matrix_size, matrix_half, matrix_center;
    float *error_matrix;
    float threshold = 127;
    if (mode == 0) { // Floyd-Steinberg
        matrix_size = 3;
        float temp_matrix[9] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 7.0f, 3.0f, 5.0f, 1.0f};
        error_matrix = new float[matrix_size * matrix_size];
        for (int i = 0; i < matrix_size * matrix_size; i++)
            error_matrix[i] = temp_matrix[i] / 16.0f;
    } else if (mode == 1) { // JJN
        matrix_size = 5;
        float temp_matrix[25] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 7.0f, 5.0f,
                                 3.0f, 5.0f, 7.0f, 5.0f, 3.0f, 1.0f, 3.0f, 5.0f, 3.0f, 1.0f};
        error_matrix = new float[matrix_size * matrix_size];
        for (int i = 0; i < matrix_size * matrix_size; i++)
            error_matrix[i] = temp_matrix[i] / 48.0f;
    } else if (mode == 2) { // Stucki
        matrix_size = 5;
        float temp_matrix[25] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f, 4.0f,
                                 2.0f, 4.0f, 8.0f, 4.0f, 2.0f, 1.0f, 2.0f, 4.0f, 2.0f, 1.0f};
        error_matrix = new float[matrix_size * matrix_size];
        for (int i = 0; i < matrix_size * matrix_size; i++)
            error_matrix[i] = temp_matrix[i] / 42.0f;
    } else {
        cerr << "Error mode" << endl;
        return 0;
    }
    matrix_half = matrix_size / 2;
    matrix_center = matrix_half * matrix_size + matrix_half;

    // Initial f~(i, j)
    float* image_f = new float[image.row * image.col];
    for (int i = 0; i < image.row * image.col; i++)
        image_f[i] = (float)image.data[i];

    // For each pixel:  error e, image b, and forward f~
    for (int i = 0; i < image.row; i++) {
        for(int j = 0; j < image.col; j++) {
            if (image_f[i * image.col + j] > threshold)
                *image.Get_Pixel(i, j, 0) = (ImgPixel)255;
            else
                *image.Get_Pixel(i, j, 0) = (ImgPixel)0;
            float coef_e = image_f[i * image.col + j] - (float)(*image.Get_Pixel(i, j, 0));
            for (int m = matrix_half; m < matrix_size; m++) {
                for (int n = 0; n < matrix_size; n++) {
                    int temp_index_x = i + m - matrix_half;
                    int temp_index_y = j + n - matrix_half;
                    if (temp_index_x >= 0 && temp_index_x < image.row && temp_index_y >= 0 && temp_index_y < image.col && (m * matrix_size + n) > matrix_center) {
                        image_f[temp_index_x * image.col + temp_index_y] += error_matrix[m * matrix_size + n] * coef_e;
                    }
                }
            }
        }
    }

    // Delete
    delete error_matrix;
    delete image_f;
    return 1;
}


//////////////////////////////////////////////////////
/////////////// Color Image Halftoning ///////////////
//////////////////////////////////////////////////////
int Halftoning::Apply_Scalar_Color_halftoning(int mode) {
    // check image
    if (image.byte != 3) {
        cerr << "image must be color" << endl;
        return 0;
    }

    // Separate to three image (CMY)
    int color_size = 255;
    ImgPixel *temp_color[3];
    temp_color[0] = new ImgPixel[image.row * image.col];
    temp_color[1] = new ImgPixel[image.row * image.col];
    temp_color[2] = new ImgPixel[image.row * image.col];
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            temp_color[0][i * image.col + j] = (ImgPixel)color_size - image.Get_Value(i, j, 0);
            temp_color[1][i * image.col + j] = (ImgPixel)color_size - image.Get_Value(i, j, 1);
            temp_color[2][i * image.col + j] = (ImgPixel)color_size - image.Get_Value(i, j, 2);
        }
    }
    Image image_C(image.row, image.col, 1, temp_color[0]);
    Image image_M(image.row, image.col, 1, temp_color[1]);
    Image image_Y(image.row, image.col, 1, temp_color[2]);

    // Apply Error Diffusion
    Halftoning halftoning_C(&image_C);
    Halftoning halftoning_M(&image_M);
    Halftoning halftoning_Y(&image_Y);
    halftoning_C.Apply_Error_Diffusion(mode);
    halftoning_M.Apply_Error_Diffusion(mode);
    halftoning_Y.Apply_Error_Diffusion(mode);

    // Combine three CMY
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            *image.Get_Pixel(i, j, 0) = (ImgPixel)color_size - halftoning_C.image.Get_Value(i, j, 0);
            *image.Get_Pixel(i, j, 1) = (ImgPixel)color_size - halftoning_M.image.Get_Value(i, j, 0);
            *image.Get_Pixel(i, j, 2) = (ImgPixel)color_size - halftoning_Y.image.Get_Value(i, j, 0);
        }
    }

    // Delete memory
    delete temp_color[0];
    delete temp_color[1];
    delete temp_color[2];
    return 1;
}


int Halftoning::Apply_Vector_Color_Halftoning(int mode) {
    // Convert to CMY image
    if (image.byte != 3) {
        cerr << "image must be RGB" << endl;
        return 0;
    }

    // Set Error Diffussion Matrix
    int matrix_size, matrix_half, matrix_center;
    float *error_matrix;
    float threshold = 127;
    if (mode == 0) { // Floyd-Steinberg
        matrix_size = 3;
        float temp_matrix[9] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 7.0f, 3.0f, 5.0f, 1.0f};
        error_matrix = new float[matrix_size * matrix_size];
        for (int i = 0; i < matrix_size * matrix_size; i++)
            error_matrix[i] = temp_matrix[i] / 16.0f;
    } else if (mode == 1) { // JJN
        matrix_size = 5;
        float temp_matrix[25] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 7.0f, 5.0f,
                                 3.0f, 5.0f, 7.0f, 5.0f, 3.0f, 1.0f, 3.0f, 5.0f, 3.0f, 1.0f};
        error_matrix = new float[matrix_size * matrix_size];
        for (int i = 0; i < matrix_size * matrix_size; i++)
            error_matrix[i] = temp_matrix[i] / 48.0f;
    } else if (mode == 2) { // Stucki
        matrix_size = 5;
        float temp_matrix[25] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f, 4.0f,
                                 2.0f, 4.0f, 8.0f, 4.0f, 2.0f, 1.0f, 2.0f, 4.0f, 2.0f, 1.0f};
        error_matrix = new float[matrix_size * matrix_size];
        for (int i = 0; i < matrix_size * matrix_size; i++)
            error_matrix[i] = temp_matrix[i] / 42.0f;
    } else {
        cerr << "Error mode" << endl;
        return 0;
    }
    matrix_half = matrix_size / 2;
    matrix_center = matrix_half * matrix_size + matrix_half;

    // Initial CMY by RGB image
    double* image_CMY = new double[image.row * image.col * image.byte];
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            for (int k = 0; k < image.byte; k++)
                image_CMY[i * image.col * image.byte + j * image.byte + k] = 255.0 - (double)(*image.Get_Pixel(i, j, k));
        }
    }

    // For each pixel:  error E, image RBG, and forward CMY by E
    double coef_e[3] = {};
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            // Find V
            int index = i * image.col * image.byte + j * image.byte;
            int *vertex = Find_Closest_Vector(image_CMY[index], image_CMY[index + 1], image_CMY[index + 2]);

            // Set value to RGB image
            (*image.Get_Pixel(i, j, 0)) = (ImgPixel)255 - (ImgPixel)vertex[0];
            (*image.Get_Pixel(i, j, 1)) = (ImgPixel)255 - (ImgPixel)vertex[1];
            (*image.Get_Pixel(i, j, 2)) = (ImgPixel)255 - (ImgPixel)vertex[2];

            // Calculate E
            coef_e[0] = image_CMY[index] - vertex[0];
            coef_e[1] = image_CMY[index + 1] - vertex[1];
            coef_e[2] = image_CMY[index + 2] - vertex[2];

            // Forward Error
            for (int m = matrix_half; m < matrix_size; m++) {
                for (int n = 0; n < matrix_size; n++) {
                    int temp_index_x = i + m - matrix_half;
                    int temp_index_y = j + n - matrix_half;
                    int temp_index = temp_index_x * image.col * image.byte + temp_index_y * image.byte;
                    if (temp_index_x >= 0 && temp_index_x < image.row && temp_index_y >= 0 && temp_index_y < image.col && (m * matrix_size + n) > matrix_center) {
                        image_CMY[temp_index] += error_matrix[m * matrix_size + n] * coef_e[0];
                        image_CMY[temp_index + 1] += error_matrix[m * matrix_size + n] * coef_e[1];
                        image_CMY[temp_index + 2] += error_matrix[m * matrix_size + n] * coef_e[2];
                    }
                }
            }
        }
    }

    // delete
    delete error_matrix;
    delete image_CMY;
    return 1;
}


////////////////////////////////////////////////
/////////////// Helper functions ///////////////
////////////////////////////////////////////////
float* Halftoning::Generate_Bayer_Matrix(int size, int color_number) {
    float *bayer_matrix = new float[size * size];
    if (size == 2) {
        float matrix[4] = {1, 2, 3, 0};
        for (int i = 0; i < size * size; i++) {
            bayer_matrix[i] = (matrix[i] + 0.5f) * (float)color_number / (size * size);
        }
    } else if (size == 4) {
        float matrix[16] = {5, 9, 6, 10, 13, 1, 14, 2, 7, 11, 4, 8, 15, 3, 12, 0};
        for (int i = 0; i < size * size; i++) {
            bayer_matrix[i] = (matrix[i] + 0.5f) * (float)color_number / (size * size);
        }
    } else if (size == 8) {
        float matrix[64] = {21, 37, 25, 41, 22, 38, 26, 42,
                            53, 5, 57, 9, 54, 6, 58, 10,
                            29, 45, 17, 33, 30, 46, 18, 34,
                            61, 13, 49, 1, 62, 14, 50, 2,
                            23, 39, 27, 43, 20, 36, 24, 40,
                            55, 7, 59, 11, 52, 4, 56, 8,
                            31, 47, 19, 35, 28, 44, 16, 32,
                            63, 15, 51, 3, 60, 12, 48, 0};
        for (int i = 0; i < size * size; i++) {
            bayer_matrix[i] = (matrix[i] + 0.5f) * (float)color_number / (size * size);
        }
    } else {
        cerr << "Wrong matrix size" << endl;
    }
    return bayer_matrix;
}

int* Halftoning::Find_Closest_Vector(double color_C, double color_M, double color_Y) {
    // Convert CMY cube
    color_C = color_C / 255.0;
    color_M = color_M / 255.0;
    color_Y = color_Y / 255.0;

    // Initial six quadrants
    int *vertex = new int[3];
    int quadrant[6][4][3] = {{{0, 1, 0}, {1, 0, 0}, {0, 0, 1}, {0, 0, 0}}, // CMYW
                             {{1, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}}, // MYGC
                             {{1, 0, 1}, {0, 1, 1}, {1, 0, 0}, {0, 0, 1}}, // RGMY
                             {{1, 1, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 0}}, // KRGB
                             {{1, 0, 1}, {0, 1, 1}, {1, 1, 0}, {1, 0, 0}}, // RGBM
                             {{0, 1, 0}, {1, 0, 0}, {0, 1, 1}, {1, 1, 0}}}; // CMGB

    // Determine the quadrant
    int quadrant_number = 0;
    if (color_C + color_M < 1) {
        if (color_M + color_Y < 1) {
            if (color_C + color_M + color_Y < 1)
                quadrant_number = 0;
            else
                quadrant_number = 1;
        }
        else
            quadrant_number = 2;
    } else {
        if (color_M + color_Y >= 1) {
            if (color_M + color_Y + color_C >= 2)
                quadrant_number = 3;
            else
                quadrant_number = 4;
        } else
            quadrant_number = 5;
    }

    // Find the closest vertex in determined quadrant
    double distance = 100;
    for (int i = 0; i < 4; i++) {
        double temp = (color_M - quadrant[quadrant_number][i][0]) * (color_M - quadrant[quadrant_number][i][0])
                      + (color_C - quadrant[quadrant_number][i][1]) * (color_C - quadrant[quadrant_number][i][1])
                      + (color_Y - quadrant[quadrant_number][i][2]) * (color_Y - quadrant[quadrant_number][i][2]);
        if (temp < distance) {
            distance = temp;
            vertex[0] = quadrant[quadrant_number][i][0] * 255;
            vertex[1] = quadrant[quadrant_number][i][1] * 255;
            vertex[2] = quadrant[quadrant_number][i][2] * 255;
        }
    }

    // Find the closest vertex in cube (For report)
    /*int cube[8][3] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {1, 0, 0},
                      {0, 1, 1}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
    double distance2 = 100;
    for (int i = 0; i < 8; i++) {
        double temp = (color_M - cube[i][0]) * (color_M - cube[i][0])
                       + (color_C - cube[i][1]) * (color_C - cube[i][1])
                       + (color_Y - cube[i][2]) * (color_Y - cube[i][2]);
        if (temp < distance2) {
            distance2 = temp;
            vertex[0] = cube[i][0] * 255;
            vertex[1] = cube[i][1] * 255;
            vertex[2] = cube[i][2] * 255;
        }
    }*/

    // Convert to CMY order
    int temp_color;
    temp_color = vertex[0];
    vertex[0] = vertex[1];
    vertex[1] = temp_color;
    return vertex;
}

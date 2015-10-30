#include "Image.h"

using namespace std;

class Halftoning {
public:
    Image image;
    // constructor
    Halftoning(int new_row, int new_col, int new_byte, string filename) {
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

    // Dithering
    float* Generate_Bayer_Matrix(int size, int color_number) {
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

        cout << endl << "Bayer_Matrix: size " << size << " " << color_number << endl;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << bayer_matrix[i * size + j] << " ";
            }
            cout << endl;
        }
        return bayer_matrix;
    }

    int Apply_Dithering(int size, int color_number) {
        float *bayer_matrix = Generate_Bayer_Matrix(size, color_number);
        int bayer_matrix_row = 0;
        int bayer_matrix_col = 0;
        int color_size = 255 / color_number;
        for (int i = 0; i < image.row; i++) {
            for (int j = 0; j < image.col; j++) {
                for (int k = 0; k < image.byte; k++) {
                    int value = (int)((int)(*image.Get_Pixel(i, j, k)) / bayer_matrix[bayer_matrix_row * size + bayer_matrix_col]);
                    if (value > color_size)
                        value = color_size;
                    *image.Get_Pixel(i, j, k) = (ImgPixel)(color_number * value);
                    bayer_matrix_col++;
                    if (bayer_matrix_col >= size)
                        bayer_matrix_col = 0;
                }
            }
            bayer_matrix_col = 0;
            bayer_matrix_row++;
            if (bayer_matrix_row >= size)
                bayer_matrix_row = 0;
        }
        return 1;
    }

    // Effor Diffusion
    int Apply_Error_Diffusion(int mode) {
        if (image.byte != 1) {
            cerr << "Image must be grayscale" << endl;
            return 0;
        }

        // Set Error Diffussion matrix
        int matrix_size, matrix_half, matrix_center;
        float *error_matrix;
        float threshold = 127;
        if (mode == 0) {
            // Floyd-Steinberg
            matrix_size = 3;
            float temp_matrix[9] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 7.0f, 3.0f, 5.0f, 1.0f};
            error_matrix = new float[matrix_size * matrix_size];
            for (int i = 0; i < matrix_size * matrix_size; i++)
                error_matrix[i] = temp_matrix[i] / 16.0f;
        } else if (mode == 1) {
            // JJN
            matrix_size = 5;
            float temp_matrix[25] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 7.0f, 5.0f,
                                    3.0f, 5.0f, 7.0f, 5.0f, 3.0f,
                                    1.0f, 3.0f, 5.0f, 3.0f, 1.0f};
            error_matrix = new float[matrix_size * matrix_size];
            for (int i = 0; i < matrix_size * matrix_size; i++)
                error_matrix[i] = temp_matrix[i] / 48.0f;
        } else if (mode == 2) {
            // Stucki
            matrix_size = 5;
            float temp_matrix[25] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 8.0f, 4.0f,
                                     2.0f, 4.0f, 8.0f, 4.0f, 2.0f,
                                     1.0f, 2.0f, 4.0f, 2.0f, 1.0f};
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
        return 1;
    }
};

void Dithering(int size, int color_number) {
    Halftoning halftoning = Halftoning(512, 512, 1, "p2_image/mandrill.raw");
    halftoning.Apply_Dithering(size, color_number);
    string filename = "Dithering_" + to_string(size) + "_" + to_string(color_number) + ".raw";
    halftoning.image.Write(&(halftoning.image), filename);
}

// @para mode
// 0 Floyd-Steinberg
// 1 JJN
// 2 Stucki
void Error_Diffusion(int mode) {
    Halftoning halftoning = Halftoning(512, 512, 1, "p2_image/mandrill.raw");
    halftoning.Apply_Error_Diffusion(mode);
    halftoning.image.Print_Data("Error", 20, 20);
    string filename = "ErrorDiffuse_" + to_string(mode) + ".raw";
    halftoning.image.Write(&(halftoning.image), filename);
}


void prob2a() {
    cout << "Problem 2a" << endl;
    int number = 0;
    switch (number) {
        case 0:
            Dithering(2, 255);
            Dithering(4, 255);
            Dithering(8, 255);
            break;
        case 1:
            Dithering(2, 85);
            Dithering(4, 85);
            Dithering(8, 85);
            break;
        case 2:
            Dithering(2, 1);
            break;
    }
}

void prob2b() {
    cout << "Problem 2b" << endl;
    Error_Diffusion(0);
    Error_Diffusion(1);
    Error_Diffusion(2);
}


int main() {
    cout << "Hello, World!" << endl;
    int number = 1;
    switch (number) {
        case 0:
            prob2a();
            break;
        case 1:
            prob2b();
            break;
    }
    return 0;
}
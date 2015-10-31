#include "Image.h"

#ifndef P2_HALFTONING_H
#define P2_HALFTONING_H

class Halftoning {
public:
    Image image;

////////////////////////////////////////////
/////////////// Constructors ///////////////
////////////////////////////////////////////
    // int new_row: image row
    // int new_col: image col
    // int new_byte: image byte
    // string filename: filename to read in
    Halftoning(int new_row, int new_col, int new_byte, string filename);

    // Image *: pointer of image to read in
    Halftoning(Image *new_image);

    ~Halftoning();


//////////////////////////////////////////////////////////
/////////////// Grayscale Image Halftoning ///////////////
//////////////////////////////////////////////////////////
    // Dithering application for grayscale image
    // int size: Dithering Matrix size (2 or 4 or 8)
    // int color_number: Range of color:
    // (0, 255): 255
    // (0, 85, 170, 255): 85
    int Apply_Dithering(int size, int color_number);

    // Error Diffusion application for grayscale image
    // int mode:
    // 0 Floyd-Steinberg
    // 1 JJN
    // 2 Stucki
    int Apply_Error_Diffusion(int mode);


//////////////////////////////////////////////////////
/////////////// Color Image Halftoning ///////////////
//////////////////////////////////////////////////////
    // Scalar Color Halftoning for RGB image
    // int mode:
    // 0 Floyd-Steinberg
    // 1 JJN
    // 2 Stucki
    int Apply_Scalar_Color_halftoning(int mode);

    // Vector Color Halftoning for RGB image
    // int mode:
    // 0 Floyd-Steinberg
    // 1 JJN
    // 2 Stucki
    int Apply_Vector_Color_Halftoning(int mode);


private:
////////////////////////////////////////////////
/////////////// Helper functions ///////////////
////////////////////////////////////////////////
    // Generate Bayer Matrix for Dithering
    // int size: matrix size
    // int color_number: range of color
    // (0, 255): 255
    // (0, 85, 170, 255): 85
    float* Generate_Bayer_Matrix(int size, int color_number);

    // Find the closest vector in the MBVQ quadrant
    // double color_C: color intensity in C color (0, 255)
    // double color_M: color intensity in M color (0, 255)
    // double color_Y: color intensity in Y color (0, 255)
    int *Find_Closest_Vector(double color_C, double color_M, double color_Y);
};
#endif //P2_HALFTONING_H

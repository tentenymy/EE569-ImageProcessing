//
// Created by Maggie on 9/3/15.
//

#ifndef HW1_HELPER_H
#define HW1_HELPER_H

#endif //HW1_HELPER_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

class Image
{
public:
    FILE *file;
    int BytesPerPixel;
    int Size;

    Image();
    ~Image();

    void Img_Input();
    void Img_Output();
    void Img_Input_Output_Argu();
};
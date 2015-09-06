#include <stdio.h>
#include <iostream>
#include "hw1_helper.h"

using namespace std;

int main(int argc, char *argv[])
{
    Info info;
    unsigned char image_data [info.width][info.height][info.byteperpixel];

    fread(image_data, sizeof(unsigned char), info.width * info.height* info.byteperpixel, info.file);
    fclose(info.file);

    Image_Plot_All_Line(&image_data[0][0][0], &info, "image_plot_all_line.txt");
    info.Info_Print();


    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    return 0;
}



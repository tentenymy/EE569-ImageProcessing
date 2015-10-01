#include "hw2_helper.h"
#include "p1a.h"

using namespace cv;
using namespace std;




class TextureClassifier
{

};

class ImageOperater
{

};

char* StringChar(string str)
{
    int length = str.length();
    char *ch = new char[length + 1];
    strcpy(ch, str.c_str());
}

void File_Read (string filename, Image *ptr_image, int height, int width, int byteperpixel)
{
    FILE *file;
    char *ch_filename = StringChar(filename);
    if (!(file = fopen(ch_filename, "rb"))) {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = height * width * byteperpixel;
    Image image[size];
    fread(image, sizeof(Image), size, file);
    fclose(file);
    ptr_image = image;
}



/*void File_Write (string filename, int height, int width, int byteperpixel)
{
    char new_filename[60];
    strcpy(new_filename, FOLDER_WRITE);
    strcat(new_filename, filename_write);
    if (!(file = fopen(new_filename, "wb")))
    {
        cout << "Cannot open file: " << filename_write <<endl;
        exit(1);
    }

    Image image[height] [width] [byteperpixel];
    memcpy(image, pt_image, sizeof(image));
    fwrite(image, sizeof(Image), (size_t)width * height * byteperpixel, file);
    fclose(file);
}*/



int main(int argc, char *argv[]){

    cout << "Hello World" << endl;



    // Test
    string filename = "p1/p1_image/p1_image_a/grass_01.raw";
    Image *ptr_image = NULL;
    File_Read(filename, ptr_image, 128, 128, 1);
    cout << ptr_image[0] << endl;




}
#include "hw2_helper.h"
#include "p1a.h"

using namespace cv;
using namespace std;

void File_Read(char*, Image*, int, int, int);
void File_Write(char*, Image*, int, int, int);


class TextureClassifier
{

};

class ImageOperater
{

};


void File_Read (char *filename, Image *image, int height, int width, int byteperpixel)
{
    FILE *file;
    if (!(file = fopen(filename, "rb")))
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }

    size_t size = height * width * byteperpixel;
    Image image_buffer[size];
    fread(image_buffer, sizeof(Image), size, file);
    fclose(file);
    image = image_buffer;
}



void File_Write (char* filename, Image *image, int height, int width, int byteperpixel)
{
    FILE *file;
    if (!(file = fopen(filename, "rb")))
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }

    size_t size = height * width * byteperpixel;
    Image image_buffer[size];
    fread(image_buffer, sizeof(Image), size, file);
    fclose(file);
    image = image_buffer;
}


void Feature_Extract ()
{
    Mat
}

int main(int argc, char *argv[]){

    cout << "Hello World" << endl;

    // Test
    char *filename = "p1_image/p1_image_a/grass_01.raw";
    Image image [128 * 128];
    File_Read(filename, image, 128, 128, 1);





    return 0;
}
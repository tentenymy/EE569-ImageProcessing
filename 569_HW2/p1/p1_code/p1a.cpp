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


Image* File_Read (char *filename, int height, int width, int byteperpixel)
{
    cout << "File_Read" << filename << endl;

    FILE *file;
    if (!(file = fopen(filename, "rb")))
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }

    size_t size = height * width * byteperpixel;
    Image image_buffer[height * width];
    fread(image_buffer, sizeof(Image), size, file);


    cout << "Test_read: " << (int)image_buffer[2] << " " <<(int)image_buffer[120 * 128] << endl;

    fclose(file);
    return image_buffer;
}

void File_Write (char* filename, Image *image, int height, int width, int byteperpixel)
{
    cout << "File_Write: " << filename << endl;

    FILE *file;
    if (!(file = fopen(filename, "wb")))
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

void Mat_Print (Mat mat, string filename)
{
    cout << "Mat_Print: " << filename << endl;

    fstream fout;
    fout.open(filename, ios_base::out);
    if (!fout.is_open())
    {
        cerr << "Cannot open file: " << filename << endl;
    }
    fout << mat << endl;
    fout.close();
}
Mat Mat_Convert (Image *image, int height, int width, int byteperpixel)
{
    cout << "Mat_Convert" << endl;

    Mat mat_image;
    mat_image.create(height, width, byteperpixel);
    memcpy(mat_image.data, image, height * width);



    for (int i = 0; i < height * width * byteperpixel; i++)
    {

    }
    return mat_image;
}

void Feature_Extract (InputArray image)
{
    cout << "Feature_Extract" << endl;



    Mat meanImage, noOffset;
    //boxFilter(mat_image, meanImage, -1, Size(5, 5));
    //subtract(image, meanImage, noOffset);
    //Mat_Print(image, "p1_result/test2.txt");


}


void Image_Print (Image *image, string filename, int height, int width, int byteperpixel)
{
    cout << "Image_Print: " << filename << endl;

    fstream fout;
    fout.open(filename, ios_base::out);
    if (!fout.is_open())
    {
        cerr << "Cannot open file: " << filename << endl;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < byteperpixel; k++)
            {
                fout << (int)image[i * width * byteperpixel + j * byteperpixel + k];
            }

        }
        fout << endl;
    }

    fout.close();
}


void Mat_File_Write_Gray(char *filename, Mat mat)
{
    cout << "Mat_File_Write_Gray: " << filename << endl;

    Image image[128 * 128];
    fstream fout;

    fout.open("testxx.txt", ios_base::out);
    if (!fout.is_open())
    {
        cerr << "Cannot open file: " << "testxx.raw" << endl;
    }

    for (int i = 0; i < 128 * 128; i++)
    {

        image[i] = (Image)mat.data[i];
        fout << (int)image[i];
        cout << "IMGAE: " << (int)image[i] << endl;
    }
    cout << "MAT: " << (int)mat.data[10] << endl;
    cout << "IMGAE: " << (int)image[0] << endl;

    fout.close();

}


int main(int argc, char *argv[]){

    cout << "Hello World" << endl;
    char filename[] = "p1_image/p1_image_a/grass_01.raw";
    Image *pt_image = File_Read(filename, 128, 128, 1);


    Image image[128 * 128];
    for (int i = 0; i < 128 * 128; i++)
        image[i] = *pt_image++;

    cout << "Test: " << (int)image[2] << " " << (int)image[120 * 128] << endl;

    Image_Print(image, "Hello.txt", 128, 128, 1);

    Mat meanImage, noOffset;


    double d_image[128 * 128];

    for (int i = 0; i < 128 * 128; i++)
    {
        d_image[i] = (double)image[i]/255.0;
    }

    Mat mat_image(128, 128, CV_8UC1, image);

    //boxFilter(mat_image, meanImage, -1, Size(5, 5));
    //subtract(mat_image, meanImage, noOffset);

    Image_Print(image, "test0.txt", 128, 128, 1);

    cout << mat_image.rows << " " << mat_image.cols << endl;
    //cout << mat_image << endl << endl;

    namedWindow ("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", mat_image);
    waitKey(0);
    return 0;
}
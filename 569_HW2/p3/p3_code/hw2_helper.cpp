/* EE569 Homework Assignment #2
 * Date: October 11, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * email: meiyiyan@usc.edu
 * Problem2. Edge Detection
 *
 *
 * Main function:
 * p2_main.cpp
 *
 * Class EdgeDetector:
 * edge_detector.h
 * edge_detector.cpp
 *
 * Class ImgMatOperator (General class for whole HW2)
 * hw2_helper.h
 * hw2_helper.cpp
 *
 * p2c: matlab code
 * prob2c.m
 * detector_SE.m
 */

#include "hw2_helper.h"

ImgMatOperator::ImgMatOperator()
{

}

Img* ImgMatOperator::Img_Raw_Read (char *filename, int height, int width, int byteperpixel)
{
    // cout << "Img_Raw_Read: " << filename << endl;
    FILE *file;
    if (!(file = fopen(filename, "rb")))
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = height * width * byteperpixel;
    Img image[size];
    fread(image, sizeof(Img), size, file);
    fclose(file);
    Img *pt_image = image;
    return pt_image;
}

Img* ImgMatOperator::Img_Raw_Read (string filename, int height, int width, int byteperpixel)
{
    // cout << "Img_Raw_Read: " << filename << endl;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    FILE *file;
    if (!(file = fopen(c_filename, "rb")))
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = height * width * byteperpixel;
    Img image[size];
    fread(image, sizeof(Img), size, file);
    fclose(file);
    Img *pt_image = image;
    return pt_image;
}

void ImgMatOperator::Img_Raw_Write (char* filename, Img *pt_image, int height, int width, int byteperpixel)
{
    cout << "Img_Raw_Write: " << filename << endl;
    FILE *file;
    if (!(file = fopen(filename, "wb")))
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = height * width * byteperpixel;
    Img image[size];
    memcpy(image, pt_image, size);
    fwrite(image, sizeof(Img), size, file);
    fclose(file);
}

void ImgMatOperator::Mat_File_Print (Mat mat, string filename)
{
    cout << "Mat_File_Print: " << filename << endl;
    fstream fout;
    fout.open(filename, ios_base::out);
    if (!fout.is_open())
    {
        cerr << "Cannot open file: " << filename << endl;
    }
    fout << mat << endl;
    fout.close();
}

void ImgMatOperator::Img_File_Print (Img *image, string filename, int height, int width, int byteperpixel)
{
    cout << "Img_File_Print: " << filename << endl;
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
                fout << (int)*image++ << " ";
        }
        fout << endl;
    }
    fout.close();
}

void ImgMatOperator::Mat_XML_Print(Mat mat, string filename)
{
    cout << "Mat_XML_Print: " << filename << endl;
    fstream fout;
    fout.open(filename, ios_base::out);
    if (!fout.is_open())
    {
        cerr << "Cannot open file: " << filename << endl;
    }
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
            fout << (int)mat.data[i * mat.cols + j] << endl;
    }
    fout.close();
}

Mat ImgMatOperator::Img_To_Mat_Convert (Img *image, int height, int width, int byteperpixel)
{
    cout << "Img_To_Mat_Convert" << endl;
    Mat mat(height, width, CV_8UC1, image);
    return mat;
}

void ImgMatOperator::Mat_Raw_Write_Gray (Mat mat, string filename)
{
    cout << "Mat_Raw_Write_Gray: " << filename << endl;
    const int width = mat.cols;
    const int height = mat.rows;
    Img image[mat.cols * mat.rows];
    fstream fout;
    fout.open(filename, ios_base::out);
    if (!fout.is_open())
        cerr << "Cannot open file: " << filename << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {
            fout << (Img) mat.at<double>(i, j);
        }
    }
    fout.close();
}

void ImgMatOperator::Mat_Raw_Write_Color (Mat mat, string filename)
{
    cout << "Mat_Raw_Write_Color: " << filename << endl;
    const int width = mat.cols;
    const int height = mat.rows;
    vector<Mat> channels(3);
    split(mat, channels);
    fstream fout;
    fout.open(filename, ios_base::out);
    if (!fout.is_open())
        cerr << "Cannot open file: " << filename << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {
            fout << (Img)channels[2].at<unsigned char>(i, j);
            fout << (Img)channels[1].at<unsigned char>(i, j);
            fout << (Img)channels[0].at<unsigned char>(i, j);
            //cout << (int)channels[0].at<unsigned char>(i, j) << " ";
        }
        //cout << endl;
    }
    fout.close();
}

Mat ImgMatOperator::Mat_Raw_Read (string filename, int height, int width, int byteperpixel) {
    cout << "Mat_Raw_Read: " << filename << endl;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    Mat mat;
    FILE *file;
    if (!(file = fopen(c_filename, "rb")))
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = height * width * byteperpixel;
    Img image[size];
    fread(image, sizeof(Img), size, file);
    fclose(file);

    if (byteperpixel == 1) {
        double d_image[size];
        int count = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                d_image[count] = (double)image[count];
                count++;
            }
        }
        mat = Mat(height, width, CV_64F, d_image).clone();
    }
    if (byteperpixel == 3) {
        cout << "RGB image" << endl;
        Img d_image[size];
        int count = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                for (int k = 0; k < byteperpixel; k++) {
                    if (k == 0) {
                        Img temp = image[count];
                        image[count] = image[count + 2];
                        image[count + 2] = temp;
                    }
                    d_image[count] = (Img)image[count];
                    count++;
                    //cout << image[count] << " ";
                }
            }
            //cout << endl;
        }
        mat = Mat(height, width, CV_8UC3, d_image).clone();
    }
    return mat;
}

int *ImgMatOperator::Get_Window(int i, int j, int height, int width, int half_window) {
    // top, bottom, left, right
    int window[4] = {i - half_window, i + half_window, j - half_window, j + 2};
    if (window[0] < 0)
        window[0] = 0;
    if (window[2] < 0)
        window[2] = 0;
    if (window[1] > height - 1)
        window[1] = height - 1;
    if (window[3] > width - 1)
        window[3] = width - 1;
    int *res = window;
    return res;
}

void ImgMatOperator::Test()
{
    cout << "Test" << endl;
    char filename[] = "p1_image/p1_image_a/grass_01.raw";
    Img *pt_image = Img_Raw_Read(filename, 128, 128, 1);
    Img image[128 * 128];
    for (int i = 0; i < 128 * 128 * 1; i++)
        image[i] = pt_image[i];
    strcpy(filename, "test_img_raw_write.raw");
    Img_Raw_Write(filename, image, 128, 128, 1);
    Img_File_Print(image, "test_img_file_print.txt", 128, 128, 1);
    Mat mat = Img_To_Mat_Convert(image, 128, 128, 1);
    Mat_File_Print(mat, "test_mat_file_print.txt");
    Mat_Raw_Write_Gray(mat, "test_mat_raw_write.raw");
    Mat_XML_Print(mat, "test_mat_xml_print.txt");

    Mat mat2 = Mat_Raw_Read("p1_image/p1_image_a/grass_01.raw", 128, 128, 1);
    // cout << "MAT2: " << mat2 << endl << endl;
}



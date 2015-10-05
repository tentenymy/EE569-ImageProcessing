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

void ImgMatOperator::Mat_Raw_Write (Mat mat, string filename)
{
    cout << "Mat_Raw_Write_Gray: " << filename << endl;
    Img image[128 * 128];
    fstream fout;

    fout.open(filename, ios_base::out);
    if (!fout.is_open())
    {
        cerr << "Cannot open file: " << "testxx.raw" << endl;
    }

    for (int i = 0; i < 128 * 128; i++)
    {
        fout << (Img)mat.data[i];
    }
    fout.close();

}

Mat ImgMatOperator::Mat_Raw_Read (string filename, int height, int width, int byteperpixel)
{
    //cout << "Mat_Raw_Read: " << filename << endl;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());

    Img *img = new Img[128 * 128];
    Img *pt_img = Img_Raw_Read(c_filename, 128, 128, 1);

    for (int i = 0; i < 128 * 128; i++)
        img[i] = *pt_img++;

    Mat mat = Mat(128, 128, CV_8UC1, img).clone();
    return mat;
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
    Mat_Raw_Write(mat, "test_mat_raw_write.raw");
    Mat_XML_Print(mat, "test_mat_xml_print.txt");

    Mat mat2 = Mat_Raw_Read("p1_image/p1_image_a/grass_01.raw", 128, 128, 1);
    // cout << "MAT2: " << mat2 << endl << endl;
}

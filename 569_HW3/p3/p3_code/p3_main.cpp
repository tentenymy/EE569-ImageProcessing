#include "Image.h"

typedef int Dataset[9];
const int PATTERN_S_SIZE = 58;
const int PATTERN_T_SIZE = 46;
const int PATTERN_K_SIZE = 40;
const int PATTERN_C_SIZE = 332;
const int M = 2;


class Morphology {
public:
    Image image;
    Dataset *filter = NULL;
    Dataset *filter2 = NULL;

    Morphology() { };
    Morphology(Image *new_image) {
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
    Morphology(int new_row, int new_col, int new_byte, ImgPixel *pt_img) {
        if (new_col <= 0 || new_col > 1024) {
            cerr << "wrong column" << endl;
            exit(1);
        }
        if (new_row <= 0 || new_row > 1024) {
            cerr << "wrong column" << endl;
            exit(1);
        }
        image.col = new_col;
        image.row = new_row;
        image.byte = new_byte;
        image.data = new ImgPixel [image.col * image.row * image.byte];
        if (!image.data) {
            cerr << "Wrong allocate memory" << endl;
            exit(1);
        }
        if (pt_img == NULL) {
            for (int i = 0; i < image.col * image.row * image.byte; i++) {
                image.data[i] = 0;
            }

        } else {
            for (int i = 0; i < image.col * image.row * image.byte; i++) {
                image.data[i] = *pt_img++;
            }
        }
    }

    ~Morphology() {
        if (filter)
            delete [] filter;
        if (filter2)
            delete [] filter2;
    }

    int Initialize_Pattern_Conditional(string filename, int size) {
        cout << "Initialize_Pattern_Conditional " << size << endl;
        ifstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Cannot open the file" << endl;
            return 0;
        }
        filter = new Dataset[size];
        char temp;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < 10; j++) {
                fout.get(temp);
                if (temp == '1' || temp == '0') {
                    filter[i][j] = (int)temp - 48;
                }
            }
        }
        return 1;
    }

    int Initialize_Pattern_Unconditional(string filename, int size) {
        cout << "Initialize_Pattern_Unconditional " << size << endl;
        ifstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Cannot open the file" << endl;
            return 0;
        }
        filter2 = new Dataset[size];
        char temp;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < 10; j++) {
                fout.get(temp);
                if (temp == '1' || temp == '0' || temp == '2') {
                    filter2[i][j] = (int)temp - 48;
                }
            }
        }
        return 1;
    }

    void Apply_Pattern() {
        // Initial Conditional Pattern and Unconditional Pattern
        Initialize_Pattern_Conditional("Pattern_S.txt", PATTERN_S_SIZE);
        //Initialize_Pattern_Conditional("Pattern_K.txt", PATTERN_K_SIZE);
        //Initialize_Pattern_Conditional("Pattern_T.txt", PATTERN_T_SIZE);
        Initialize_Pattern_Unconditional("Pattern_Conditional.txt", PATTERN_C_SIZE);

        ImgPixel imgpixel[48] = {0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        1, 1, 0, 0, 0, 0, 0, 0,
                        1, 1, 0, 1, 1, 1, 1, 0,
                        0, 0, 0, 0, 0, 0, 1, 0,
                        0, 0, 0, 0, 0, 0, 0, 0};

        int row = 6;
        int col = 8;
        string filename = "p3_image/Horseshoe.raw";

        Image image = Image(row, col, 1, imgpixel);
        Image image_M = Image(row, col, 1, imgpixel);
        image.Print_Data("Image_ori", 10, 10);
        image_M.Print_Data("Image_M_ori", 10, 10);


        int index[2][9] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1}, {-1, 0, 1, -1, 0, 1, -1, 0, 1}};

        /*for (int i = 0; i < row * col; i++) {
            image.data[i] /= 255;
            image_M.data[i] /= 255;
        }*/

        int is_change = 1;
        int count = 0;
        while(count < 10 && is_change == 1) {
            is_change = 0;
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if (image.Get_Value(i, j, 0) == 1) {
                        // Find neighbor dataset
                        Dataset pixel_neighbor = {};
                        for (int k = 0; k < 9; k++) {
                            pixel_neighbor[k] = (int)image.Get_Value(i + index[0][k], j + index[1][k], 0);
                        }
                        // Compare the unconditional pattern and set M image
                        for (int k = 0; k < PATTERN_S_SIZE; k++) {
                            int res = Compare_Dataset(pixel_neighbor, filter[k]);
                            if (res == 1) {
                                *image_M.Get_Pixel(i, j, 0) = (ImgPixel)2;
                                is_change = 1;
                                break;
                            }
                        }
                    }
                }
            }
            image_M.Print_Pattern_Data("ImageM");


            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if (image_M.Get_Value(i, j, 0) == 2) {
                        *image.Get_Pixel(i, j, 0) = (ImgPixel)0;

                        // Find neighbor dataset
                        Dataset pixel_neighbor = {};
                        for (int k = 0; k < 9; k++) {
                            pixel_neighbor[k] = (int)image_M.Get_Value(i + index[0][k], j + index[1][k], 0) / 2;
                            cout << pixel_neighbor[k] << " ";
                        }
                        cout << endl;

                        // Compare the conditional pattern and set M image
                        int res = 0;
                        for (int k = 0; k < PATTERN_C_SIZE; k++) {
                            res = Compare_Dataset(pixel_neighbor, filter2[k]);

                            if (res == 1) {
                                cout << "(" << i << ", " << j << ")" << k << ": ";
                                *image.Get_Pixel(i, j, 0) = (ImgPixel)1;

                                for (int m = 0; m < 9; m++) {
                                    if (image_M.Get_Value(i + index[0][m], j + index[1][m], 0) == 2 && index[0][m] != 0 && index[1][m] != 0) {
                                        cout << "SET (" << i + index[0][m] << ", " << j + index[1][m] << ")" << endl;
                                        *image.Get_Pixel(i + index[0][m], j + index[1][m], 0) = (ImgPixel)1;
                                        break;
                                    }
                                }
                                break;
                            }

                        }
                        //image.Print_Pattern_Data("ImageF");
                        if (res == 0) {
                            *image.Get_Pixel(i, j, 0) = (ImgPixel)0;

                        }
                    }

                }
            }

            for (int i = 0; i < row * col; i++) {
                image_M.data[i] = image.data[i];
            }
            count++;
            cout << "Count: " << count << " Change: " << is_change << endl;
            image.Print_Pattern_Data("Image");
        }




        //image.Print_Pattern_Data("ImageFinal");

        for (int i = 0; i < row * col; i++) {
            image.data[i] *= 255;
        }
        //image.Print_Pattern_Data("imag");
        //image.Write("test.raw");

        Image new_image(row, col, 1, "test.raw");
        //new_image.Print_Pattern_Data("");
    };

    int Compare_Dataset(Dataset data1, Dataset data2) {
        int res = 1;
        for (int i = 0; i < 9; i++) {
            if (data1[i] != data2[i])
                res = 0;
        }
        res = 1;
        //cout << res;
        return res;
    }
};


void prob3a() {
    Morphology op = Morphology();
    op.Apply_Pattern();


}

int main() {
    cout << "Homework 3.3" << endl;
    prob3a();
    return 0;
}
/* EE569 Homework Assignment #4
 * Date: Noverber 29, 2015
 * Name: Meiyi Yang
 * ID: 6761054585
 * email: meiyiyan@usc.edu
 * Problem1. OCR
 *
 * Main function:
 * p1_main.cpp
 *
 * Class OCR:
 * OCR.h
 * OCR.cpp
 *
 * Class Image:
 * image.h
 * image.cpp
 *
 * Class Morphology:
 * Morphology.h
 * Morphology.cpp
 */
#include "Morphology.h"

///////////////////////////////////////
///////////// Constructor /////////////
///////////////////////////////////////
Morphology::Morphology() { };

Morphology::Morphology(Image *new_image) {
    // Set value to image
    image.col = new_image->col;
    image.row = new_image->row;
    image.byte = 1;
    image.data = new ImgPixel[image.col * image.row * image.byte];
    if (!image.data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }
    for (int i = 0; i < image.col * image.row; i++) {
        image.data[i] = new_image->data[i];
    }
    // Set to binary image
    Convert_to_Binary_Image();
}

Morphology::Morphology(int new_row, int new_col, int new_byte, ImgPixel *pt_img) {
    // Check parameter
    if (new_col <= 0 || new_col > 1024) {
        cerr << "wrong column" << endl;
        exit(1);
    }
    if (new_row <= 0 || new_row > 1024) {
        cerr << "wrong column" << endl;
        exit(1);
    }
    if (new_byte != 1) {
        cerr << "Image must be grayscale" << endl;
        exit(1);
    }
    // Set value to image
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
    // Set to binary image
    //Convert_to_Binary_Image();
    is_binary = 1;
}

Morphology::~Morphology() {
        if (filter)
            delete [] filter;
        if (filter2)
            delete [] filter2;
    }


////////////////////////////////////////////
///////////// S/T/K Morphology /////////////
////////////////////////////////////////////
int Morphology::Apply_Hit_Miss(int size_filter1, int size_filter2) {
    int PRINT = 0;
    // Convert to binary image
    int row = image.row;
    int col = image.col;
    if (!is_binary)
        Convert_to_Binary_Image();

    // Hit-or-Miss
    int index[2][9] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1}, {-1, 0, 1, -1, 0, 1, -1, 0, 1}};
    int is_change = 1;
    int count = 0;
    while(count < 100 && is_change == 1) {
        // For statistics and debug
        is_change = 0;
        int count_M = 0;
        int count_F = 0;
        // Generate M image by Hit-or-Miss filter1
        Image image_M = Image(row, col, 1, NULL);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (image.Get_Value(i, j, 0) == 1) {
                    // Find neighbor dataset
                    Dataset pixel_neighbor = {};
                    for (int k = 0; k < 9; k++)
                        pixel_neighbor[k] = (int)image.Get_Value(i + index[0][k], j + index[1][k], 0);
                    // Compare the conditional pattern
                    int res = 0;
                    int k = 0;
                    for (k = 0; k < size_filter1; k++) {
                        res = Compare_Dataset_Conditional(pixel_neighbor, filter[k]);
                        if (res == 1)
                            break;
                    }
                    // Set M image
                    if (res == 1) { // Hit
                        *image_M.Get_Pixel(i, j, 0) = (ImgPixel)1;
                        count_F++;
                    }
                }
            }
        }
        if (PRINT == 1)
            image_M.Print_Pattern_Data("M");

        // Hit-or Miss filter2
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (image_M.Get_Value(i, j, 0) == 1) {
                    // Find neighbor dataset
                    Dataset pixel_neighbor = {};
                    for (int k = 0; k < 9; k++)
                        pixel_neighbor[k] = (int)image_M.Get_Value(i + index[0][k], j + index[1][k], 0);
                    // Compare the conditional pattern and set M image
                    int res = 0;
                    int k = 0;
                    for (k = 0; k < size_filter2; k++) {
                        res = Compare_Dataset_Unconditional(pixel_neighbor, filter2[k]);
                        if (res == 1)
                            break;
                    }
                    // Set value to Image F
                    if (res == 0) { // Miss
                        count_M++;
                        is_change = 1;
                        *image.Get_Pixel(i, j, 0) = (ImgPixel)0;
                    }
                }
            }
        }
        count++;
        if (PRINT == 1)
            image.Print_Pattern_Data("F");
    }

    // Calculate Pixel number for debug
    int count_pixel = 0;
    for (int i = 0; i < row * col; i++) {
        if (image.data[i] > 0)
            count_pixel++;
    }
    //cout << "   COUNT: " << count_pixel << " ROUND: " << count << endl;
    return 0;
};

int Morphology::Operator_Hit_Miss(string filename1, string filename2, int size1, int size2) {
    /*if (size1 == PATTERN_S1)
        cout << "Operator_Hit_Miss (Shrinking)" << endl;
    else if (size1 == PATTERN_S2)
        cout << "Operator_Hit_Miss (Thinning)" << endl;
    else
        cout << "operator_Hit_Miss (Sketonizing)" << endl;*/
    Initialize_Pattern_Conditional(filename1, size1);
    Initialize_Pattern_Unconditional(filename2, size2);
    Apply_Hit_Miss(size1, size2);
    return 1;
}


///////////////////////////////////////////////////
///////////// Dilate/Erode Morphology /////////////
///////////////////////////////////////////////////
int Morphology::Operator_Dilate(Dataset filter) {
    //cout << "Operator_Dilate: ";
    int size = 9;
    for (int i = 0; i < size; i++)
        //cout << filter[i] << " ";
    //cout << endl;
    if (!is_binary)
        Convert_to_Binary_Image();

    // Find pixels to be added by filter
    int neighbor_index[2][9] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1}, {-1, 0, 1, -1, 0, 1, -1, 0, 1}};
    vector<int> dilate_index_list;
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            if (image.Get_Value(i, j, 0) == 1) {
                for (int k = 0; k < size; k++) {
                    if (filter[k] == 1) {
                        int temp_x = i + neighbor_index[0][k];
                        int temp_y = j + neighbor_index[1][k];
                        if (temp_x >= 0 && temp_x < image.row && temp_y >= 0 && temp_y < image.col)
                            dilate_index_list.push_back(temp_x * image.col + temp_y);
                    }
                }
            }
        }
    }
    // Set dilated pixels
    for (int i = 0; i < dilate_index_list.size(); i++) {
        image.data[dilate_index_list[i]] = (ImgPixel)1;
    }
    return 1;
}

int Morphology::Operator_Erode(Dataset filter) {
    //cout << "Operator_Erode: ";
    int size = 9;
    for (int i = 0; i < size; i++)
        //cout << filter[i] << " ";
    //cout << endl;
    if (!is_binary)
        Convert_to_Binary_Image();

    // Find pixels to be added by filter
    int neighbor_index[2][9] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1}, {-1, 0, 1, -1, 0, 1, -1, 0, 1}};
    vector<int> erode_index_list;
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            if (image.Get_Value(i, j, 0) == 1) {
                // Find neighbor dataset
                Dataset pixel_neighbor = {};
                for (int k = 0; k < size; k++) {
                    pixel_neighbor[k] = (int)image.Get_Value(i + neighbor_index[0][k], j + neighbor_index[1][k], 0);
                }
                // Compare the filter and pixels
                int res = 1;
                for (int k = 0; k < size; k++) {
                    if (filter[k] == 1) {
                        if (filter[k] != pixel_neighbor[k]) {
                            res = 0;
                            break;
                        }
                    }
                }
                // Add pixel
                if (res == 1) {
                    erode_index_list.push_back(i * image.col + j);
                }
            }
        }
    }
    // Set erode pixels
    for (int i = 0; i < image.row * image.col; i++) {
        image.data[i] = (ImgPixel)0;
    }
    for (int i = 0; i < erode_index_list.size(); i++) {
        image.data[erode_index_list[i]] = (ImgPixel)1;
    }
    return 1;
}

int Morphology::Operator_Open(Dataset filter1, Dataset filter2) {
    Operator_Erode(filter1);
    Operator_Dilate(filter2);
    return 1;
}

int Morphology::Operator_Close(Dataset filter1, Dataset filter2) {
    Operator_Dilate(filter1);
    Operator_Erode(filter2);
    return 1;
}

int Morphology::Operator_Filter(Dataset filter) {
    //cout << "Operator_Filter: ";
    int size = 9;
    for (int i = 0; i < size; i++)
        cout << filter[i] << " ";
    cout << endl;
    if (!is_binary)
        Convert_to_Binary_Image();

    // Find pixels to be added by filter
    int neighbor_index[2][9] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1}, {-1, 0, 1, -1, 0, 1, -1, 0, 1}};
    vector<int> erode_index_list;
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            if (image.Get_Value(i, j, 0) == 1) {
                // Find neighbor dataset
                Dataset pixel_neighbor = {};
                for (int k = 0; k < size; k++) {
                    pixel_neighbor[k] = (int)image.Get_Value(i + neighbor_index[0][k], j + neighbor_index[1][k], 0);
                }
                // Compare the filter and pixels
                int res = 1;
                for (int k = 0; k < size; k++) {
                    if (filter[k] != pixel_neighbor[k]) {
                        res = 0;
                        break;
                    }
                }
                // Add pixel
                if (res == 1) {
                    erode_index_list.push_back(i * image.col + j);
                }
            }
        }
    }
    // Set erode pixels
    for (int i = 0; i < image.row * image.col; i++) {
        image.data[i] = (ImgPixel)0;
    }
    for (int i = 0; i < erode_index_list.size(); i++) {
        image.data[erode_index_list[i]] = (ImgPixel)1;
    }
    return 1;

}


///////////////////////////////////////////////
///////////// Counting and Display/////////////
///////////////////////////////////////////////
int Morphology::Convert_Black_Image() {
    if (!is_binary)
        Convert_to_Binary_Image();
    for (int i = 0; i < image.row * image.col; i++) {
        if (image.data[i] == 0)
            image.data[i] = 1;
        else
            image.data[i] = 0;
    }
    return 1;
}

int Morphology::Write(string filename) {
    if (is_binary)
        Convert_to_Grayscale_Image();
    image.Write(filename);
    Convert_to_Binary_Image();
    return 1;
}

int Morphology::Count_Pixel() {
    int count = 0;
    for (int i = 0; i < image.row * image.col; i++){
        if (image.data[i] > 0)
            count++;
    }
    cout << endl << "The image has " << count << " points" << endl;
    return count;
}

int Morphology::Count_Pathway(Image *image_label) {
    vector<int> label_table;
    // First Pass
    int neighbor_index[2][9] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1}, {-1, 0, 1, -1, 0, 1, -1, 0, 1}};
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            if (image.Get_Value(i, j, 0) == 1) {
                // Find neighbor
                Dataset pixel_neighbor = {};
                int temp_min = INT_MAX;
                int temp_max = 0;
                for (int k = 0; k < 4; k++) {
                    pixel_neighbor[k] = (int)image_label->Get_Value(i + neighbor_index[0][k], j + neighbor_index[1][k], 0);
                    if (pixel_neighbor[k] > temp_max)
                        temp_max = pixel_neighbor[k];
                    if (pixel_neighbor[k] != 0 && pixel_neighbor[k] < temp_min)
                        temp_min = pixel_neighbor[k];
                }

                if (temp_max == 0) {
                    // Neighbor is empty
                    int label_number = label_table.size() + 1;
                    *image_label->Get_Pixel(i, j, 0) = (ImgPixel)label_number;
                    label_table.push_back(label_number);
                } else {
                    *image_label->Get_Pixel(i, j, 0) = (ImgPixel)temp_min;
                    int min = label_table[temp_min - 1];
                    for (int k = 0; k < 4; k++) {
                        if (pixel_neighbor[k] != 0) {
                            label_table[pixel_neighbor[k] - 1] = min;
                        }
                    }
                }
            }
        }
    }

    //Process table
    for (int i = 0; i < label_table.size(); i++) {
        if ((i + 1) > label_table[i]) {
            for (int j = i; j < label_table.size(); j++) {
                if (label_table[j] == (i + 1)) {
                    label_table[j] = label_table[i];
                }
            }
        }
    }

    // Print count result
    int count = 2;
    vector <int> temp_number;
    temp_number.push_back(label_table[0]);
    for (int i = 1; i < label_table.size(); i++) {
        int temp_value = label_table[i];
        int res = 0;
        for (int j = 0; j < temp_number.size(); j++) {
            if (temp_value == temp_number[j]) {
                res = 1;
                break;
            }
        }
        if (res == 0) {
            temp_number.push_back(temp_value);
        }
    }

    // Normalize the label
    for (int i = 0; i < label_table.size(); i++) {
        for (int j = 0; j < temp_number.size(); j++) {
            if (label_table[i] == temp_number[j]) {
                label_table[i] = (j + 1);
            }
        }
    }

    // Second Pass
    for (int i = 0; i < image.row; i++) {
        for (int j = 0; j < image.col; j++) {
            if (image.Get_Value(i, j, 0) != 0) {
                int label_number = image_label->Get_Value(i, j, 0);
                *image_label->Get_Pixel(i, j, 0) = (ImgPixel)label_table[label_number - 1];
            }
        }
    }
    return temp_number.size();
}


///////////////////////////////////////////////////
///////////// S/T/K Morphology Private/////////////
///////////////////////////////////////////////////
int Morphology::Initialize_Pattern_Conditional(string filename, int size) {
    ifstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Cannot open the file " << filename << endl;
        return 0;
    }
    filter = new Dataset[size];
    char temp;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 10; j++) {
            fout.get(temp);
            if (temp == '1' || temp == '0')
                filter[i][j] = (int)temp - 48;
        }
    }
    fout.close();
    return 1;
}

int Morphology::Initialize_Pattern_Unconditional(string filename, int size) {
    ifstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Cannot open the file " << filename << endl;
        return 0;
    }
    filter2 = new Dataset[size];
    char temp;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 10; j++) {
            fout.get(temp);
            if (temp == '1' || temp == '0' || temp == '2')
                filter2[i][j] = (int)temp - 48;
        }
    }
    fout.close();
    return 1;
}

int Morphology::Compare_Dataset_Conditional(Dataset data1, Dataset data2) {
    for (int i = 0; i < 9; i++) {
        if (data1[i] != data2[i])
            return 0;
    }
    return 1;
}

int Morphology::Compare_Dataset_Unconditional(Dataset data1, Dataset data2) {
    for (int i = 0; i < 9; i++) {
        int temp1 = data1[i];
        int temp2 = data2[i];
        if (temp2 == 2)
            temp2 = 1;
        if (temp1 != temp2)
            return 0;
    }
    return 1;
}


///////////////////////////////////////////
///////////// Helper Function /////////////
///////////////////////////////////////////
int Morphology::Convert_to_Binary_Image() {
    for (int i = 0; i < image.row * image.col; i++) {
        if (image.data[i] > 250)
            image.data[i] /= 255;
    }

    is_binary = 1;
    return 1;
}

int Morphology::Convert_to_Grayscale_Image() {
    for (int i = 0; i < image.row * image.col; i++)
        image.data[i] *= 255;
    is_binary = 0;
    return 1;
}




#include "Image.h"

typedef int Dataset[9];

const int PATTERN_S1 = 58;
const int PATTERN_T1 = 46;
const int PATTERN_K1 = 40;
const int PATTERN_S2 = 332;
const int PATTERN_T2 = 332;
const int PATTERN_K2 = 348;
const string FILE_S1 = "p3_image/Pattern_S1.txt";
const string FILE_T1 = "p3_image/Pattern_T1.txt";
const string FILE_K1 = "p3_image/Pattern_K1.txt";
const string FILE_S2 = "p3_image/Pattern_S2.txt";
const string FILE_T2 = "p3_image/Pattern_T2.txt";
const string FILE_K2 = "p3_image/Pattern_K2.txt";


class Morphology {
public:
    Image image;
    Dataset *filter = NULL;
    Dataset *filter2 = NULL;
    int is_binary = 0;

///////////////////////////////////////
///////////// Constructor /////////////
///////////////////////////////////////
    Morphology() { };
    Morphology(Image *new_image) {
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
    Morphology(int new_row, int new_col, int new_byte, ImgPixel *pt_img) {
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
        Convert_to_Binary_Image();
    }
    ~Morphology() {
        if (filter)
            delete [] filter;
        if (filter2)
            delete [] filter2;
    }

////////////////////////////////////////////
///////////// S/T/K Morphology /////////////
////////////////////////////////////////////
    int Initialize_Pattern_Conditional(string filename, int size) {
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
    int Initialize_Pattern_Unconditional(string filename, int size) {
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
    int Apply_Hit_Miss(int size_filter1, int size_filter2) {
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
        cout << "   COUNT: " << count_pixel << " ROUND: " << count << endl;
        return 0;
    };
    int Operator_Hit_Miss(string filename1, string filename2, int size1, int size2) {
        if (size1 == PATTERN_S1)
            cout << "Operator_Hit_Miss (Shrinking)" << endl;
        else if (size1 == PATTERN_S2)
            cout << "Operator_Hit_Miss (Thinning)" << endl;
        else
            cout << "operator_Hit_Miss (Sketonizing)" << endl;
        Initialize_Pattern_Conditional(filename1, size1);
        Initialize_Pattern_Unconditional(filename2, size2);
        Apply_Hit_Miss(size1, size2);
        return 1;
    }

///////////////////////////////////////////
///////////// Helper Function /////////////
///////////////////////////////////////////

    int Compare_Dataset_Conditional(Dataset data1, Dataset data2) {
        for (int i = 0; i < 9; i++) {
           if (data1[i] != data2[i])
                return 0;
        }
        return 1;
    }
    int Compare_Dataset_Unconditional(Dataset data1, Dataset data2) {
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
    int Convert_to_Binary_Image() {
        for (int i = 0; i < image.row * image.col; i++)
            image.data[i] /= 255;
        is_binary = 1;
        return 1;
    }
    int Convert_to_Grayscale_Image() {
        for (int i = 0; i < image.row * image.col; i++)
            image.data[i] *= 255;
        is_binary = 0;
        return 1;
    }
    int Convert_Black_Image() {
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
    int Write(string filename) {
        if (is_binary)
            Convert_to_Grayscale_Image();
        image.Write(filename);
        Convert_to_Binary_Image();
        return 1;
    }
    int Count_Pixel() {
        int count = 0;
        for (int i = 0; i < image.row * image.col; i++){
            if (image.data[i] > 0)
                count++;
        }
        cout << endl << "The image has " << count << " points" << endl;
        return count;
    }

////////////////////////////////////////////
///////////// Structure Filter /////////////
////////////////////////////////////////////
    int Operator_Dilate(Dataset filter) {
        cout << "Operator_Dilate: ";
        int size = 9;
        for (int i = 0; i < size; i++)
            cout << filter[i] << " ";
        cout << endl;
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
    int Operator_Erode(Dataset filter) {
        cout << "Operator_Erode: ";
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
    int Operator_Open(Dataset filter1, Dataset filter2) {
        Operator_Erode(filter1);
        Operator_Dilate(filter2);
        return 1;
    }
    int Operator_Close(Dataset filter1, Dataset filter2) {
        Operator_Dilate(filter1);
        Operator_Erode(filter2);
        return 1;
    }
    int Operator_Filter(Dataset filter) {
        cout << "Operator_Filter: ";
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

    int Count_Pathway() {
        vector<vector<int>> label_table;
        Image image_label = Image(image.row, image.col, 1, NULL);
        // First Pass
        int neighbor_index[2][9] = {{-1, -1, -1, 0, 0, 0, 1, 1, 1}, {-1, 0, 1, -1, 0, 1, -1, 0, 1}};
        int label_number = 0;
        for (int i = 0; i < image.row; i++) {
            for (int j = 0; j < image.col; j++) {
                if (image.Get_Value(i, j, 0) == 1) {
                    // Find neighbor
                    Dataset pixel_neighbor = {};
                    int temp_min = INT_MAX;
                    int temp_max = 0;
                    //cout << "Neigh: " << endl;
                    for (int k = 0; k < 4; k++) {
                        pixel_neighbor[k] = (int)image_label.Get_Value(i + neighbor_index[0][k], j + neighbor_index[1][k], 0);
                        //cout << pixel_neighbor[k] << " ";
                        if (pixel_neighbor[k] > temp_max)
                            temp_max = pixel_neighbor[k];
                        if (pixel_neighbor[k] != 0 && pixel_neighbor[k] < temp_min)
                            temp_min = pixel_neighbor[k];
                    }

                    // Neighbor is empty
                    if (temp_max == 0) {
                        label_number++;
                        *image_label.Get_Pixel(i, j, 0) = (ImgPixel)label_number;
                        vector<int> temp_vector;
                        temp_vector.push_back(label_number);
                        label_table.push_back(temp_vector);
                    } else {
                    // Neighbor not empty
                        *image_label.Get_Pixel(i, j, 0) = (ImgPixel)temp_min;
                        for (int k = 0; k < 4; k++) {
                            if (pixel_neighbor[k] != 0 && pixel_neighbor[k] != temp_min) {
                                int temp_index1 = temp_min - 1;
                                int temp_index2 = pixel_neighbor[k] - 1;
                                label_table[temp_index1].push_back(pixel_neighbor[k]);
                                label_table[temp_index2].push_back(temp_min);
                                for (int m = 0; m < label_table[temp_index1].size(); m++) {
                                    label_table[temp_index2].push_back(label_table[temp_index1][m]);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Process table
        for (int i = 0; i < label_table.size(); i++) {
            int temp_min = INT_MAX;
            for (int j = 0; j < label_table[i].size(); j++) {
                int index = label_table[i][j];
                if (temp_min > index)
                    temp_min = index;
            }
            int temp = label_table[i][0];
            label_table[i][0] = temp_min;
            label_table[i].push_back(temp);
        }
        for (int i = 0; i < label_table.size(); i++) {
            int temp_min = label_table[i][0];
            if (temp_min != (i + 1) && temp_min > 1) {
                int temp_min2 = label_table[temp_min - 1][0];
                label_table[i][0] = temp_min2;
            }
        }

        // Print count result
        int count = 2;
        vector <int> temp_number;
        temp_number.push_back(label_table[0][0]);
        for (int i = 1; i < label_table.size(); i++) {
            int temp_value = label_table[i][0];
            int res = 0;
            for (int j = 0; j < temp_number.size(); j++) {
                if (temp_value == temp_number[j]) {
                    res = 1;
                    break;
                }
            }
            if (res == 0) {
                temp_number.push_back(temp_value);
                label_table[i][0] = count++;
            }
        }
        cout << "The image has " << temp_number.size() << " pathways";

        // Second Pass
        for (int i = 0; i < image.row; i++) {
            for (int j = 0; j < image.col; j++) {
                if (image.Get_Value(i, j, 0) != 0) {
                    int value = image_label.Get_Value(i, j, 0);
                    int temp_min = label_table[value - 1][0];
                    *image_label.Get_Pixel(i, j, 0) = (ImgPixel)temp_min;
                }
            }
        }
        for (int i = 0; i < image.row * image.col; i++)
            image_label.data[i] *= 25;
        image_label.Write("Find_Object_2Count.raw");
        return 1;
    }
};

///////////////////////////////////////
///////////// Application /////////////
///////////////////////////////////////
void Test_Shrinking(string filename_read, string filename_write, int row, int col) {
    Image image = Image(row, col, 1, filename_read);
    Morphology op = Morphology(&image);
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write(filename_write);
}

void Test_Thinning(string filename_read, string filename_write, int row, int col) {
    Image image = Image(row, col, 1, filename_read);
    Morphology op = Morphology(&image);
    op.Operator_Hit_Miss(FILE_T1, FILE_T2, PATTERN_T1, PATTERN_T2);
    op.Write(filename_write);
}

void Test_Skeletonizing(string filename_read, string filename_write, int row, int col) {
    Image image = Image(row, col, 1, filename_read);
    Morphology op = Morphology(&image);
    op.Operator_Hit_Miss(FILE_K1, FILE_K2, PATTERN_K1, PATTERN_K2);
    op.Write(filename_write);
}

void Test() {
    cout << "Test" << endl;
    //Initial test
    int row = 10;
    int col = 10;
    ImgPixel temp_imgpixel[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                  1, 1, 0, 0, 0, 0, 1, 0, 0, 1,
                                  0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
                                  0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
                                  0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < row * col; i++)
        temp_imgpixel[i] *= 255;
    Image image = Image(row, col, 1, temp_imgpixel);
    //Image image = Image(108, 91, 0, "p3_image/Horseshoe.raw");
    Morphology op = Morphology(&image);
    op.image.Print_Data("Original");
    op.image.Print_Pattern_Data("Original");
    /*op.Initialize_Pattern_Conditional("p3_image/Pattern_S.txt", PATTERN_S_SIZE);
    op.Initialize_Pattern_Unconditional("p3_image/Pattern_Uncoditional_ST.txt", PATTERN_C_ST_SIZE);
    op.image.Print_Pattern_Data("Origianl");
    op.Apply_Hit_Miss(1, PATTERN_S_SIZE, PATTERN_C_ST_SIZE);
    op.Write("test.raw");
    op.image.Print_Pattern_Data("");*/

    Dataset filter1 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    Dataset filter2 = {0, 1, 0, 1, 1, 1, 0, 1, 0};

    /*// Dilate

    op.Operator_Dilate(filter1);
    op.image.Print_Pattern_Data("Dilate");
    //op.image.Print_Data("Dilate");



    op.Operator_Erode(filter2);
    op.image.Print_Pattern_Data("Erode");
    //op.image.Print_Data("Erode");*/

    /*op.Operator_Open(filter1, filter2);
    op.image.Print_Pattern_Data("Open");*/

    op.Operator_Close(filter1, filter2);
    op.image.Print_Pattern_Data("Close");
};

void Find_Nails() {
    // Initialization
    cout << "Find Nails" << endl;
    Image image = Image(108, 91, 1, "p3_image/Horseshoe.raw");
    Morphology op = Morphology(&image);

    // Pre-processing: Erode
    Dataset filter1 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    op.Operator_Erode(filter1);
    op.Write("Find_Nails_1Erode.raw");

    // Shrinking
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write("Find_Nails_2Shrink.raw");

    // Nail Filter
    Dataset filter2 = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    op.Operator_Filter(filter2);
    op.Write("Find_Nails_3Filter.raw");
    op.Count_Pixel();
};

void Find_Holes() {
    // Initialization
    cout << "Find Holes" << endl;
    Image image = Image(108, 91, 1, "p3_image/Horseshoe.raw");
    Morphology op = Morphology(&image);

    // Pre-processing: Convert image white to black
    op.Convert_Black_Image();
    op.Write("Find_Holes_1Black.raw");

    // Shrinking
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write("Find_Holes_2Shrink.raw");

    // Nail Filter
    Dataset filter1 = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    op.Operator_Filter(filter1);
    op.Write("Find_Holes_3Filter.raw");
    op.Count_Pixel();
}

void Find_Objects() {
    // Initialization
    cout << "Find Objects" << endl;
    Image image = Image(108, 91, 1, "p3_image/Horseshoe.raw");
    Morphology op = Morphology(&image);

    // Pre-processing: Convert image white to black
    Dataset filter1 = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Dataset filter2 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    Dataset filter3 = {0, 1, 0, 0, 1, 0, 0, 0, 0};
    Dataset filter4 = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    op.Operator_Open(filter1, filter2);
    op.Operator_Dilate (filter3);
    op.Write("Find_Object_1OpenDilate.raw");


    // Shrinking
    op.Operator_Hit_Miss(FILE_S1, FILE_S2, PATTERN_S1, PATTERN_S2);
    op.Write("Find_Object_2Shrinking.raw");
    op.Count_Pathway();
}


void prob3a() {
    cout << "Problem 3a" << endl;
    string filename = "p3_image/Horseshoe.raw";
    int row = 108;
    int col = 91;
    int number = 2;
    switch(number) {
        case 0:
            Find_Nails();
            break;
        case 1:
            Find_Holes();
            break;
        case 2:
            Find_Objects();
        case 3:
            Test_Shrinking(filename, "Test_Shrinking.raw", row, col);
            Test_Thinning(filename, "Test_Thinning.raw", row, col);
            Test_Skeletonizing(filename, "Test_Skeletonizing.raw", row, col);
            break;
        case 4:
            Test();
            break;
    }
}

void Apply_Thinning(int number_close, int number_dilate) {
    string filename = "p3_image/Horse1.raw";
    int row = 275;
    int col = 360;
    Image image = Image(row, col, 1, filename);
    Morphology op = Morphology(&image);

    // Pre-processing
    Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Dataset filter2 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    for (int i = 0; i < number_close; i++)
        op.Operator_Close(filter, filter2);
    for (int i = 0; i < number_dilate; i++)
        op.Operator_Dilate(filter);

    // Thinning
    op.Operator_Hit_Miss(FILE_T1, FILE_T2, PATTERN_T1, PATTERN_T2);
    string filename_write = "Thinning_" + to_string(number_close) + "_" + to_string(number_dilate) + ".raw";
    op.Write(filename_write);
}

void Apply_Skeletonizing(int number_close, int number_dilate) {
    string filename = "p3_image/Horse1.raw";
    int row = 275;
    int col = 360;
    Image image = Image(row, col, 1, filename);
    Morphology op = Morphology(&image);

    // Pre-processing
    Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Dataset filter2 = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    for (int i = 0; i < number_close; i++)
        op.Operator_Close(filter, filter2);
    for (int i = 0; i < number_dilate; i++)
        op.Operator_Dilate(filter);

    // Thinning
    op.Operator_Hit_Miss(FILE_K1, FILE_K2, PATTERN_K1, PATTERN_K2);
    string filename_write = "Skeletonizing_" + to_string(number_close) + "_" + to_string(number_dilate) + ".raw";
    op.Write(filename_write);
}

void prob3b() {
    cout << "Problem 3b" << endl;
    int number = 4;
    switch(number) {
        case 0:
            Apply_Thinning(0, 0);
            break;
        case 1:
            Apply_Thinning(2, 0);
            Apply_Thinning(2, 1);
            Apply_Thinning(2, 2);
            Apply_Thinning(2, 3);
            Apply_Thinning(2, 4);
            Apply_Thinning(2, 5);
            break;
        case 2:
            Apply_Thinning(0, 2);
            Apply_Thinning(1, 2);
            Apply_Thinning(2, 2);
            Apply_Thinning(3, 2);
            Apply_Thinning(4, 2);
            Apply_Thinning(5, 2);
            break;
        case 3:
            Apply_Skeletonizing(0, 0);
            break;
        case 4:
            Apply_Skeletonizing(2, 0);
            Apply_Skeletonizing(2, 1);
            Apply_Skeletonizing(2, 2);
            Apply_Skeletonizing(2, 3);
            Apply_Skeletonizing(2, 4);
            Apply_Skeletonizing(2, 5);
            //break;
        case 5:
            Apply_Skeletonizing(0, 2);
            Apply_Skeletonizing(1, 2);
            Apply_Skeletonizing(2, 2);
            Apply_Skeletonizing(3, 2);
            Apply_Skeletonizing(4, 2);
            Apply_Skeletonizing(5, 2);
            break;
    }
}

int main() {
    cout << "Homework 3.3" << endl;
    //prob3a();
    prob3b();
    return 0;
}
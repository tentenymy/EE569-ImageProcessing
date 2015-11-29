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

#include "OCR.h"

/////////////////////////////////////////
////////////// Constructor //////////////
/////////////////////////////////////////
OCR::OCR(int new_row, int new_col, int new_byte, string filename, int new_pathway_number, string *new_label) {
        if (new_col <= 0 || new_col > 1024) {
            cerr << "Wrong column" << endl;
            exit(1);
        }
        if (new_row <= 0 || new_row > 1024) {
            cerr << "Wrong row" << endl;
            exit(1);
        }
        image.col = new_col;
        image.row = new_row;
        image.byte = new_byte;
        char *c_filename = new char[filename.length() + 1];
        strcpy(c_filename, filename.c_str());
        FILE *file;
        if (!(file = fopen(c_filename, "rb")))  {
            cout << "Cannot open file: " << filename << endl;
            exit(1);
        }
        image.data = new ImgPixel[image.col * image.row * image.byte];
        if (!image.data) {
            cerr << "Wrong allocate memory" << endl;
            exit(1);
        }
        fread(image.data, sizeof(ImgPixel), image.col * image.row * image.byte, file);
        fclose(file);
        if (c_filename)
            delete c_filename;
        Convert_To_Gray(&image);
        Convert_To_Binary_Turn(&image);

        // Label
        pathway_number = new_pathway_number;
        label = new string[pathway_number];
        for (int i = 0; i < pathway_number; i++) {
            label[i] = new_label[i];
        }
    }


//////////////////////////////////////
////////////// Training //////////////
//////////////////////////////////////
void OCR::Train(string filename, int new_row, int new_col) {
    cout << "Train" << endl;
    Segment_Train(filename, new_row, new_col);
    //Process_Train(filename, new_row, new_col);
    Feature_Train(filename, new_row, new_col);
}

void OCR::Segment_Train(string filename, int new_row, int new_col) {
    cout << "Segment_Train" << endl;
    // Label pathway
    Morphology morphology(&image);
    Image image_label(image.row, image.col, 1, NULL);
    pathway_number = morphology.Count_Pathway(&image_label);

    // Segment by connecting pathway
    for (int m = 0; m < pathway_number; m++) {
        Image image_new(image.row, image.col, 1, NULL);
        for (int i = 0; i < image.row; i++) {
            for (int j = 0; j < image.col; j++) {
                if ((int)image_label.Get_Value(i, j, 0) == (m + 1))
                    *image_new.Get_Pixel(i, j, 0) = (ImgPixel)255;
            }
        }

        // resize to 100 * 100
        int row_max = 0;
        int row_min = INT_MAX;
        int col_max = 0;
        int col_min = INT_MAX;
        for (int i = 0; i < image.row; i++) {
            for (int j = 0; j < image.col; j++) {
                if ((int)image_new.Get_Value(i, j, 0) > 0) {
                    if (i > row_max)
                        row_max = i;
                    if (i < row_min)
                        row_min = i;
                    if (j > col_max)
                        col_max = j;
                    if (j < col_min)
                        col_min = j;
                }
            }
        }
        Image image_resize((row_max - row_min + 1), (col_max - col_min + 1), 1, NULL);
        for (int i = 0; i < (row_max - row_min + 1); i++) {
            for (int j = 0; j < (col_max - col_min + 1); j++) {
                if ((int)image_new.Get_Value(i + row_min, j + col_min, 0) > 0) {
                    *image_resize.Get_Pixel(i, j, 0) = (ImgPixel)255;
                }
            }
        }
        label_row = new_row;
        label_col = new_col;
        filename_train = filename;
        image_resize.Convert_Size(new_row, new_col);

        // Write
        Convert_To_Binary(&image_resize);
        image_resize.Write(filename + to_string(m) + ".raw");
    }
}

void OCR::Feature_Train(string filename, int new_row, int new_col) {
    cout << "Feature_Train" << endl;
    Initial_Bit_Quad();
    cout << "Label\t" << "Area\t" << "Peri \t" << "Circle  \t" << "SymmetrX\t" << "SymmetrY\t" << "MomentX  \t" << "MomentY  \t"<< "Euler\t" << endl;
    int count = 0;
    for (int i = 0; i < pathway_number; i++) {
        // Symmetry feature
        double feature_area = Feature_Area(filename, i);
        double feature_symmetry_X = Feature_Symmetry_X(filename, i, feature_area);
        double feature_symmetry_Y = Feature_Symmetry_Y(filename, i, feature_area);

        // Thinning
        Image image_feature(label_row, label_col, 1, filename + to_string(i) + ".raw");
        Morphology op = Morphology(&image_feature);
        Dataset filter = {0, 1, 0, 1, 1, 1, 0, 1, 0};
        Dataset filter2 = {0, 0, 0, 0, 1, 0, 0, 0, 0};
        for (int i = 0; i < 3; i++)
            op.Operator_Close(filter, filter);
        for (int i = 0; i < 3; i++)
            op.Operator_Dilate(filter);
        op.Operator_Hit_Miss(FILE_T1, FILE_T2, PATTERN_T1, PATTERN_T2);
        op.image.Write(filename + to_string(i) + ".raw");

        // other feature
        feature_area = Feature_Area(filename, i);
        double feature_perimeter = Feature_Perimeter(filename, i);
        double feature_euler = Feature_Euler(filename, i);
        double feature_circularity = Feature_Circularity(filename, i, feature_area, feature_perimeter);
        double feature_moment_X = Feature_Moment_X(filename, i, feature_area);
        double feature_moment_Y = Feature_Moment_Y(filename, i, feature_area);
        string res = Classifier(feature_euler, feature_symmetry_X, feature_symmetry_Y, feature_moment_X, feature_moment_Y, feature_area, feature_perimeter);

        // Print
        cout << label[i] << "    \t";
        cout << feature_area << "    \t";
        cout << feature_perimeter << "    \t";
        cout << feature_circularity << " \t";
        cout << feature_symmetry_X << "     \t";
        cout << feature_symmetry_Y << "     \t";
        cout << feature_moment_X << "   \t";
        cout << feature_moment_Y << "   \t";
        cout << feature_euler << "   \t";
        cout << res;
        if (res == label[i]) {
            count++;
        } else {
            cout << " (*)";
        }
        cout << endl;
    }
    cout << "Total " << count << ", " << pathway_number << ", " << (int)(((double)count / pathway_number) * 100) << " %" << endl;
}

string OCR::Classifier(double feature_euler, double feature_symmetry_X, double feature_symmetry_Y, double feature_moment_X, double feature_moment_Y, double feature_area, double feature_perimeter) {
    string res = "";
    if (feature_area < 100) {
        return "I";
    }
    if (feature_euler > 0) {
        if (feature_symmetry_X > 0.87) {
            if (feature_moment_X > 0.5)
                return "M";
            else
                return "T";
        } else if (feature_symmetry_X > 0.47 && feature_symmetry_X <= 0.87) {
            if (feature_symmetry_Y > 0.80) {
                return "E";
            } else if (feature_symmetry_Y > 0.6 && feature_symmetry_Y <= 0.9) {
                if (feature_moment_Y > 0.55)
                    return "3";
                else if (feature_moment_X > 0.52)
                    return "2";
                else
                    return "S";
            } else if (feature_symmetry_Y > 0.35 && feature_symmetry_Y <= 0.6) {
                if (feature_moment_X > 0.5)
                    return "2";
                else
                    return "5";
            } else
                return "7";
        } else {
            if (feature_moment_X > 0.5)
                return "L";
            else
                return "1";
        }
    } else if (feature_euler == 0) {
        if (feature_symmetry_X > 0.89) {
            return "0";
        } else {
            if (feature_symmetry_Y > 0.85) {
                return "D";
            } else {
                if (feature_moment_X > 0.44) {
                    if (feature_moment_Y > 0.5) {
                        if (feature_perimeter > 630)
                            return "9";
                        else
                            return "4";
                    } else
                        return "6";
                } else
                    return "P";
            }
        }
    } else
        return "8";
}


/////////////////////////////////////
////////////// Testing //////////////
/////////////////////////////////////
void OCR::Test(int new_row, int new_col, int new_byte, string filename_test, string *label_test, int threshold1, int threshold2) {
    Segment_Test(filename_test, &image_test, threshold1, threshold2);
    vector<double> vector_symmetry_X, vector_symmetry_Y;
    Feature_Test_Symmetry(filename_test, &vector_symmetry_X, &vector_symmetry_Y);
    Process_Test(filename_test, label_test);
    Feature_Test(filename_test, label_test, &vector_symmetry_X, &vector_symmetry_Y);
}

void OCR::Segment_Test(string filename_test, Image *new_image, int threshold1, int threshold2) {
    cout << "Segment_Test: " << endl;
    // Label pathway
    Morphology morphology(new_image);
    Image image_label(new_image->row, new_image->col, 1, NULL);
    pathway_number_test = morphology.Count_Pathway(&image_label);
    cout << "Total " << pathway_number_test << " pathways" << endl;

    // Segment by connecting pathway
    int count = 0;
    for (int m = 0; m < pathway_number_test; m++) {
        Image image_new(new_image->row, new_image->col, 1, NULL);
        for (int i = 0; i < new_image->row; i++) {
            for (int j = 0; j < new_image->col; j++) {
                if ((int)image_label.Get_Value(i, j, 0) == (m + 1))
                    *image_new.Get_Pixel(i, j, 0) = (ImgPixel)255;
            }
        }

        // resize to 100 * 100
        int row_max = 0;
        int row_min = INT_MAX;
        int col_max = 0;
        int col_min = INT_MAX;
        for (int i = 0; i < new_image->row; i++) {
            for (int j = 0; j < new_image->col; j++) {
                if ((int)image_new.Get_Value(i, j, 0) > 0) {
                    if (i > row_max)
                        row_max = i;
                    if (i < row_min)
                        row_min = i;
                    if (j > col_max)
                        col_max = j;
                    if (j < col_min)
                        col_min = j;
                }
            }
        }
        int row_resize = row_max - row_min + 1;
        int col_resize = col_max - col_min + 1;
        //cout << "SIZE: " << row_resize << ", " << col_resize;
        if ((row_resize + col_resize >= 2 * threshold1) && row_resize < threshold2 && col_resize < threshold2) {
            //cout << " (*)";
            Image image_resize(row_resize, col_resize, 1, NULL);
            for (int i = 0; i < (row_max - row_min + 1); i++) {
                for (int j = 0; j < (col_max - col_min + 1); j++) {
                    if ((int)image_new.Get_Value(i + row_min, j + col_min, 0) > 0) {
                        *image_resize.Get_Pixel(i, j, 0) = (ImgPixel)255;
                    }
                }
            }

            image_resize.Convert_Size(label_row, label_col);

            // binary the image
            for (int i = 0; i < image_resize.row * image_resize.col; i++) {
                ImgPixel temp = image_resize.data[i];
                if (temp > (ImgPixel)170) {
                    image_resize.data[i] = (ImgPixel)1;
                } else {
                    image_resize.data[i] = (ImgPixel)0;
                }
            }
            image_resize.Write(filename_test + "_test_" + to_string(count) + ".raw");
            //image_resize.Print_Pattern_Data("");
            count++;
        }
        //cout << endl;
    }
    pathway_number_test = count;


}

void OCR::Feature_Test_Symmetry(string filename_test, vector<double>* vector_symmetry_X, vector<double>* vector_symmetry_Y) {
    cout << "Feature_Test_Symmetry" << endl;
    for (int i = 0; i < pathway_number_test; i++) {
        // symmetry
        double feature_area = Feature_Area(filename_test + "_test_", i);
        vector_symmetry_X->push_back(Feature_Symmetry_X(filename_test + "_test_", i, feature_area));
        vector_symmetry_Y->push_back(Feature_Symmetry_Y(filename_test + "_test_", i, feature_area));
    }
}

void OCR::Feature_Test(string filename, string *new_label_test, vector<double>* vector_symmetry_X, vector<double>* vector_symmetry_Y) {
    cout << "Feature_Test" << endl;
    // Label
    string *label_test = new string[pathway_number_test];
    for (int i = 0; i < pathway_number_test; i++) {
        label_test[i] = new_label_test[i];
    }

    Initial_Bit_Quad();
    int count = 0;
    cout << "Label\t" << "Area\t" << "Peri \t" << "Circle  \t" << "SymmetrX\t" << "SymmetrY\t" << "MomentX  \t" << "MomentY  \t"<< "Euler\t" << "Result\t" << endl;
    for (int i = 0; i < pathway_number_test; i++) {
        // symmetry
        vector<double> vector_symmetry_X_temp = *(vector_symmetry_X);
        vector<double> vector_symmetry_Y_temp = *(vector_symmetry_Y);
        double feature_area = Feature_Area(filename + "_test_", i);
        double feature_symmetry_X = vector_symmetry_X_temp[i];
        double feature_symmetry_Y = vector_symmetry_Y_temp[i];
        double feature_perimeter = Feature_Perimeter(filename + "_test_", i);
        double feature_euler = Feature_Euler(filename + "_test_", i);
        double feature_circularity = Feature_Circularity(filename + "_test_", i, feature_area, feature_perimeter);
        double feature_moment_X = Feature_Moment_X(filename + "_test_", i, feature_area);
        double feature_moment_Y = Feature_Moment_Y(filename + "_test_", i, feature_area);
        string res = Classifier(feature_euler, feature_symmetry_X, feature_symmetry_Y, feature_moment_X, feature_moment_Y, feature_area, feature_perimeter);

        // Print
        cout << label_test[i] << "    \t";
        cout << feature_area << "    \t";
        cout << feature_perimeter << "  \t";
        cout << feature_circularity << " \t";
        cout << feature_symmetry_X << "  \t";
        cout << feature_symmetry_Y << "  \t";
        cout << feature_moment_X << "   \t";
        cout << feature_moment_Y << "   \t";
        cout << feature_euler << "   \t";
        cout << res;
        if (res == label_test[i]) {
            count++;
        } else {
            cout << " (*)";
        }
        cout << endl;
    }
    cout << "Total " << count << ", " << pathway_number_test << ", " << (int)(((double)count / pathway_number_test) * 100) << " %" << endl;
}

void OCR::Read_Test(int new_row, int new_col, int new_byte, string filename) {
    if (new_col <= 0 || new_col > 1024) {
        cerr << "Wrong column" << endl;
        exit(1);
    }
    if (new_row <= 0 || new_row > 1024) {
        cerr << "Wrong row" << endl;
        exit(1);
    }
    image_test.col = new_col;
    image_test.row = new_row;
    image_test.byte = new_byte;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    FILE *file;
    if (!(file = fopen(c_filename, "rb")))  {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    image_test.data = new ImgPixel[image_test.col * image_test.row * image_test.byte];
    if (!image_test.data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }
    fread(image_test.data, sizeof(ImgPixel), image_test.col * image_test.row * image_test.byte, file);
    fclose(file);
    if (c_filename)
        delete c_filename;
}

void OCR::Process_Test(string filename_test, string *new_label_test) {
    cout << "Process_Test" << endl;
    // Label
    string *label_test = new string[pathway_number_test];
    for (int i = 0; i < pathway_number_test; i++) {
        label_test[i] = new_label_test[i];
        // Thinning
        Image image_feature(label_row, label_col, 1, filename_test + "_test_" + to_string(i) + ".raw");
        Morphology op = Morphology(&image_feature);
        Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        for (int i = 0; i < 3; i++)
            op.Operator_Close(filter, filter);
        for (int i = 0; i < 3; i++)
            op.Operator_Dilate(filter);
        op.Operator_Hit_Miss(FILE_T1, FILE_T2, PATTERN_T1, PATTERN_T2);
        op.image.Write(filename_test + "_test_" + to_string(i) + ".raw");
    }
}


////////////////////////////////////////
////////////// Preprocess //////////////
////////////////////////////////////////
void OCR::Preprocess_Test1(string filename_test) {
    cout << "Preprocess_Test" << endl;
    Convert_To_Gray(&image_test);
    int max = 0;
    int min = 255;
    for (int i = 0; i < image_test.row * image_test.col; i++) {
        int temp = (int)image_test.data[i];
        if (temp > 60)
            image_test.data[i] = 0;
        else
            image_test.data[i] = 255;
    }

    image_test.Write("test.raw");
}

void OCR::Preprocess_Test2(string filename_test) {
    cout << "Preprocess_Test" << endl;
    Convert_To_Gray(&image_test);
    image_test.Write_Txt("test.txt");
    int max = 0;
    int min = 255;
    for (int i = 0; i < image_test.row * image_test.col; i++) {
        int temp = (int)image_test.data[i];
        if (temp > 170)
            image_test.data[i] = 0;
        else
            image_test.data[i] = 255;
    }



    Morphology op = Morphology(&image_test);
    Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    for (int i = 0; i < 3; i++)
        op.Operator_Close(filter, filter);

    for (int i = 0; i < image_test.row  * image_test.col; i++) {
        image_test.data[i] = op.image.data[i];
    }
    image_test.Write("test.raw");
}

void OCR::Preprocess_Test3(string filename_test) {
    cout << "Preprocess_Test3" << endl;
    Convert_To_Gray(&image_test);
    image_test.Write_Txt("test.txt");
    image_test.Write("test.raw");

    // Histogram equalization
    double pdf[256] = {};
    double cdf[256];
    int temp;
    for (int i = 0; i < image_test.row * image_test.col; i++) {
        temp = (int)image_test.data[i];
        pdf[temp]++;
    }
    for (int i = 0; i < 256; i++)
        pdf[i] /= (double)image_test.row * image_test.col;
    cdf[0] = pdf[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + pdf[i];
    }
    short tranferfunc[256];
    for (int i = 0; i < 256; i++)
        tranferfunc[i] = (short)floor(cdf[i] * 256);
    for (int i = 0; i < image_test.row; i++) {
        for (int j = 0; j < image_test.col; j++) {
            temp = (int)image_test.data[i * image_test.col + j];
            image_test.data[i * image_test.col + j] = tranferfunc[temp];
        }
    }
    image_test.Write("test2.raw");

    // binary
    int max = 0;
    int min = 255;
    for (int i = 0; i < image_test.row * image_test.col; i++) {
        int temp = (int)image_test.data[i];
        if (temp > 110)
            image_test.data[i] = 0;
        else
            image_test.data[i] = 255;
    }
    image_test.Write("test3.raw");


    Morphology op = Morphology(&image_test);
    Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    for (int i = 0; i < 3; i++)
        op.Operator_Close(filter, filter);

    for (int i = 0; i < image_test.row  * image_test.col; i++) {
        image_test.data[i] = op.image.data[i];
    }
}

void OCR::Preprocess_Test4(string filename_test) {
    cout << "Preprocess_Test3" << endl;
    Convert_To_Gray(&image_test);
    image_test.Write_Txt("test.txt");
    image_test.Write("test.raw");

    // Binary
    int max = 0;
    int min = 255;
    for (int i = 0; i < image_test.row * image_test.col; i++) {
        int temp = (int)image_test.data[i];
        if (temp > 50)
            image_test.data[i] = 0;
        else
            image_test.data[i] = 255;
    }
    image_test.Write("test2.raw");

    // Morphology
    Morphology op = Morphology(&image_test);
    Dataset filter = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Dataset filter2 = {0, 0, 0, 1, 1, 1, 0, 0, 0};
    for (int i = 0; i < 2; i++)
        op.Operator_Dilate(filter2);

    for (int i = 0; i < image_test.row  * image_test.col; i++) {
        image_test.data[i] = op.image.data[i];
    }
    for (int i = 0; i < image_test.row * image_test.col; i++) {
        image_test.data[i] *= 255;
    }
    image_test.Write("test3.raw");
    for (int i = 0; i < image_test.row * image_test.col; i++) {
        image_test.data[i] /= 255;
    }
}


//////////////////////////////////////////////
////////////// Feature Analysis //////////////
//////////////////////////////////////////////
double OCR::Feature_Area(string filename, int num) {
        Image image_feature(label_row, label_col, 1, filename + to_string(num) + ".raw");
        int total = 0;
        for (int i = 0; i < label_row; i++) {
            for (int j = 0; j < label_col; j++) {
                if (image_feature.Get_Value(i, j, 0) == (ImgPixel)1) {
                    total++;
                }
            }
        }
        return (double)total;
    }

double OCR::Feature_Perimeter(string filename, int num) {
        Image image_feature(label_row, label_col, 1, filename + to_string(num) + ".raw");
        int total = 0;
        for (int i = 0; i < label_row; i++) {
            for (int j = 0; j < label_col; j++) {
                if (image_feature.Get_Value(i, j, 0) == (ImgPixel)1) {
                    if (image_feature.Get_Value(i - 1, j, 0) == (ImgPixel)0)
                        total++;
                    if (image_feature.Get_Value(i + 1, j, 0) == (ImgPixel)0)
                        total++;
                    if (image_feature.Get_Value(i, j - 1, 0) == (ImgPixel)0)
                        total++;
                    if (image_feature.Get_Value(i, j + 1, 0) == (ImgPixel)0)
                        total++;
                }
            }
        }
        return (double)total;
    }

double OCR::Feature_Circularity(string filename, int num, double feature_area, double feature_perimeter) {
        double feature_cirularity = 4 * PI * feature_area / (feature_perimeter * feature_perimeter);
        return feature_cirularity;
    }

double OCR::Feature_Symmetry_Y(string filename, int num, double feature_area) {
        Image image_feature(label_row, label_col, 1, filename + to_string(num) + ".raw");
        int total = 0;
        for (int i = 0; i < label_row / 2; i++) {
            for (int j = 0; j < label_col; j++) {
                if (image_feature.Get_Value(i, j, 0) == (ImgPixel)1) {
                    if (image_feature.Get_Value(i, j, 0) == image_feature.Get_Value(label_row - 1 - i, j, 0))
                        total++;
                }
            }
        }
        return total * 2 / feature_area;
    }

double OCR::Feature_Symmetry_X(string filename, int num, double feature_area) {
    Image image_feature(label_row, label_col, 1, filename + to_string(num) + ".raw");
    int total = 0;
    for (int i = 0; i < label_row; i++) {
        for (int j = 0; j < label_col / 2; j++) {
            if (image_feature.Get_Value(i, j, 0) == (ImgPixel)1) {
                if (image_feature.Get_Value(i, j, 0) == image_feature.Get_Value(i, label_col - 1 - j, 0))
                    total++;
            }
        }
    }
    return total * 2 / feature_area;
}

double OCR::Feature_Euler(string filename, int num) {
    Image image_feature(label_row, label_col, 1, filename + to_string(num) + ".raw");
    int q1 = 0;
    int q3 = 0;
    int qD = 0;
    for (int i = 0; i < label_row; i++) {
        for (int j = 0; j < label_row; j++) {
            Bit_Quad neighbor;
            neighbor[0] = (int)image_feature.Get_Value(i, j, 0);
            neighbor[1] = (int)image_feature.Get_Value(i, j + 1, 0);
            neighbor[2] = (int)image_feature.Get_Value(i + 1, j, 0);
            neighbor[3] = (int)image_feature.Get_Value(i + 1, j + 1, 0);
            for (int k = 0; k < 4; k++) {
                q1 += Compare_Bit_Quad(neighbor, bit_quad[k]);
            }
            for (int k = 4; k < 8; k++) {
                q3 += Compare_Bit_Quad(neighbor, bit_quad[k]);
            }
            for (int k = 8; k < 10; k++) {
                qD += Compare_Bit_Quad(neighbor, bit_quad[k]);
            }
        }
    }
    double feature_Euler = 0.25 * (q1 - q3 - 2 * qD);
    return feature_Euler;
}

double OCR::Feature_Moment_X(string filename, int num, double feature_area) {
    Image image_feature(label_row, label_col, 1, filename + to_string(num) + ".raw");
    double total = 0;
    for (int i = 0; i < label_row; i++) {
        for (int j = 0; j < label_col; j++) {
            total += (i + 1.5) * image_feature.Get_Value(i, j, 0) / label_row;
        }
    }
    return total / feature_area;
}

double OCR::Feature_Moment_Y(string filename, int num, double feature_area) {
    Image image_feature(label_row, label_col, 1, filename + to_string(num) + ".raw");
    double total = 0;
    for (int i = 0; i < label_row; i++) {
        for (int j = 0; j < label_col; j++) {
            total += (j + 1.5) * image_feature.Get_Value(i, j, 0) / label_col;
        }
    }
    return total / feature_area;
}


/////////////////////////////////////////////
////////////// Helper Function //////////////
/////////////////////////////////////////////
void OCR::Initial_Bit_Quad() {
    int temp_data[40] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
                         1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            bit_quad[i][j] = temp_data[i * 4 + j];
        }
    }
}

int OCR::Compare_Bit_Quad(Bit_Quad bitquad1, Bit_Quad bitquad2) {
    for (int i = 0; i < 4; i++) {
        if (bitquad1[i] != bitquad2[i]) {
            return 0;
        }
    }
    return 1;
}

void OCR::Convert_To_Binary_Turn(Image* new_image) {
    for (int i = 0; i < new_image->row * new_image->col; i++) {
        ImgPixel temp = new_image->data[i];
        if (temp > (ImgPixel)170) {
            new_image->data[i] = (ImgPixel)0;
        } else {
            new_image->data[i] = (ImgPixel)255;
        }
    }
}

void OCR::Convert_To_Binary(Image* new_image) {
    for (int i = 0; i < new_image->row * new_image->col; i++) {
        ImgPixel temp = new_image->data[i];
        if (temp > (ImgPixel)170) {
            new_image->data[i] = (ImgPixel)1;
        } else {
            new_image->data[i] = (ImgPixel)0;
        }
    }
}

void OCR::Convert_To_Gray(Image* new_image) {
    if (new_image->byte == 3) {
        ImgPixel *data_temp = new ImgPixel [new_image->col * new_image->row];
        for (int i = 0; i < new_image->row; i++) {
            for (int j = 0; j < new_image->col; j++) {
                data_temp[i * new_image->col + j] = new_image->Get_Value(i, j, 0) * 0.21 + new_image->Get_Value(i, j, 1) * 0.72 + new_image->Get_Value(i, j, 2) * 0.07;
            }
        }
        delete new_image->data;
        new_image->data = new ImgPixel[new_image->col * new_image->row];
        for (int i = 0; i < new_image->row; i++) {
            for (int j = 0; j < new_image->col; j++) {
                new_image->data[i * new_image->col + j] = data_temp[i * new_image->col + j];
            }
        }
        new_image->byte = 1;
    }
}



#include "classifer.h"


// Declaration of default parameter
const string LABEL[5] = {"grass", "straw", "sand", "leather", "unknown"};
const string FOLDER1 = "p1_image/p1_image_a/";
const string FOLDER2 = "p1_image/p1_image_b/";


// Declaration of functions
string Convert_Filename (string, int);
void Get_List_Filename_Label_a (vector<string>*, vector<string>*, vector<string>*, vector<string>*);
void Get_List_Filename_Label_b (vector<string>*, vector<string>*, vector<string>*, vector<string>*, int*);
void Prob1a();
void Prob1b();


// Functions for get filename
string Convert_Filename (string label, int num) {
    string str_num;
    num++;
    if (num < 10)
        str_num = "0" + to_string(num);
    else
        str_num = to_string(num);
    return label + "_" + str_num + ".raw";
}

void Get_List_Filename_Label_a (vector <string> *list_filename_train, vector <string> *list_label_train,
                                    vector <string> *list_filename_test, vector <string> *list_label_test) {
    // Get filename and label list for training set
    for (int i = 0; i < 72; i++) {
        if (i < 36) {
            list_label_train->push_back(LABEL[0]);
            list_filename_train->push_back(FOLDER1 + Convert_Filename(LABEL[0], i));
        }
        else {
            list_label_train->push_back(LABEL[1]);
            list_filename_train->push_back(FOLDER1 + Convert_Filename(LABEL[1], i - 36));
        }
    }
    // Get filename and label list for testing set
    string temp_list_label_test[24] =
            {"straw", "straw", "grass", "grass", "grass", "straw",
             "grass", "straw", "straw", "grass", "grass", "grass",
             "straw", "grass", "straw" , "straw", "straw", "straw",
             "grass", "grass", "grass", "straw", "straw", "grass"};
    for (int i = 0; i < 24; i++)
    {
        list_label_test->push_back(temp_list_label_test[i]);
        list_filename_test->push_back(FOLDER1 + Convert_Filename(LABEL[4], i));
    }
}

void Get_List_Filename_Label_b (vector <string> *list_filename_train, vector <string> *list_label_train,
                                    vector <string> *list_filename_test, vector <string> *list_label_test,
                                    int* pt_index_label) {
    // Clear vector list
    list_filename_train->clear();
    list_filename_test->clear();
    list_label_train->clear();
    list_label_test->clear();
    int index_label[4];
    for (int i = 0; i < 4; i++) {
        index_label[i] = *pt_index_label++;
    }
    // Get filename and label list for training set
    for (int i = 0; i < 36; i++) {
        list_label_train->push_back(LABEL[index_label[0]]);
        list_filename_train->push_back(FOLDER2 + Convert_Filename(LABEL[index_label[0]], i));
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 12; j++) {
            list_label_train->push_back(LABEL[index_label[i + 1]]);
            list_filename_train->push_back(FOLDER2 + Convert_Filename(LABEL[index_label[i + 1]], j));
        }
    }
    // Get filename and label list for testing set
    string temp_list_label_test[12] = {LABEL[index_label[0]]};
    for (int i = 36; i < 48; i++)
    {
        list_label_test->push_back(LABEL[index_label[0]]);
        list_filename_test->push_back(FOLDER2 + Convert_Filename(LABEL[index_label[0]], i));
    }
}


// Function to implement problem 1
void Prob1a()
{
    cout << "Problem1a" << endl;
    vector <string> list_filename_train;
    vector <string> list_label_train;
    vector <string> list_filename_test;
    vector <string> list_label_test;
    Get_List_Filename_Label_a(&list_filename_train, &list_label_train, &list_filename_test, &list_label_test);
    Classifier classifier = Classifier(list_filename_train, list_label_train,
                                       list_filename_test, list_label_test);
    // problem1a 1): Feature Extract
    classifier.Set_Feature();

    // problem1a 2): MM
    classifier.Classify_MM(MODE_MM, 0);
    classifier.Print_Error_Rate();

    // problem1a 3 & 5): PCA, dimension = 1
    classifier.Classify_MM(MODE_PCA, 1);
    classifier.Print_Error_Rate();

    // problem1a 3 & 5): PCA, dimension = 2
    classifier.Classify_MM(MODE_PCA, 2);
    classifier.Print_Error_Rate();

    // problem1a 4 & 5): PCA, dimension = 1
    classifier.Classify_MM(MODE_LDA, 1);
    classifier.Print_Error_Rate();
}

void Prob1b() {
    cout << "Problem1b" << endl;
    vector<string> list_filename_train;
    vector<string> list_label_train;
    vector<string> list_filename_test;
    vector<string> list_label_test;
    int index_label[4][4] = {{0, 1, 2, 3},
                             {1, 0, 2, 3},
                             {2, 0, 1, 3},
                             {3, 0, 1, 2}};
    // problem1b 2): Minimum distance (PDA)
    for (int i = 0; i < 4; i++) {
        cout << "----------------------" << LABEL[i] << "--------------------" << endl;
        Get_List_Filename_Label_b(&list_filename_train, &list_label_train,
                                  &list_filename_test, &list_label_test, index_label[i]);
        Classifier classifier = Classifier(list_filename_train, list_label_train,
                                           list_filename_test, list_label_test);
        classifier.Set_Feature();
        classifier.Classify_MM(MODE_PCA, 3);
        classifier.Print_Error_Rate();
    }

    // problem1b 2): Minimum distance (LDA)
    for (int i = 0; i < 4; i++) {
        cout << "----------------------" << LABEL[i] << "--------------------" << endl;
        Get_List_Filename_Label_b(&list_filename_train, &list_label_train,
                                  &list_filename_test, &list_label_test, index_label[i]);
        Classifier classifier = Classifier(list_filename_train, list_label_train,
                                           list_filename_test, list_label_test);
        classifier.Set_Feature();
        classifier.Classify_MM(MODE_LDA, 3);
        classifier.Print_Error_Rate();
    }

    // Problem1b 3): SVM (PDA)
    for (int i = 0; i < 4; i++) {
        cout << "----------------------" << LABEL[i] << "--------------------" << endl;
        Get_List_Filename_Label_b(&list_filename_train, &list_label_train,
                                  &list_filename_test, &list_label_test, index_label[i]);
        Classifier classifier = Classifier(list_filename_train, list_label_train,
                                           list_filename_test, list_label_test);
        classifier.Set_Feature();
        classifier.Classify_SVM(MODE_PCA, 3);
        classifier.Print_Error_Rate();
    }
}

int main(int argc, char *argv[])
{
    cout << "Problem 1" << endl;
    Prob1a();
    //Prob1b();
    return 0;
}
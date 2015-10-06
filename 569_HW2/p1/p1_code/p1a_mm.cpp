#include "classifer.h"


// Get file and label
const string LABEL[5] = {"grass", "straw", "sand", "leather", "unknown"};
const int GRASS = 0;
const int STRAW = 1;
const int SAND = 2;
const int LEATHER = 3;
const int UNKOWN = 4;

const string FOLDER1 = "p1_image/p1_image_a/";
const string FOLDER2 = "p1_image/p1_image_b/";

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
        list_filename_test->push_back(FOLDER1 + Convert_Filename(LIST_LABEL[2], i));
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
    classifier.Print_Label();
    classifier.Set_Feature();

    classifier.Classify_MM(MODE_MM, 0);
    classifier.Print_Error_Rate();

    classifier.Classify_MM(MODE_PCA, 1);
    classifier.Print_Error_Rate();

    classifier.Classify_MM(MODE_PCA, 2);
    classifier.Print_Error_Rate();

    classifier.Classify_MM(MODE_LDA, 1);
    classifier.Print_Error_Rate();
}

void ClassifierSVM() {


    int labels[4] = {1, -1, -1, -1};
    float trainingData[4][3] = {{0.09, 0.00, 0.01},
                                {-0.03, -0.06, -0.05},
                                {0.04, 0.12, -0.02},
                                {-0.04, 0.11, 0.03}};
    Mat trainingDataMat(4, 3, CV_32FC1, trainingData);
    Mat labelsMat(4, 1, CV_32SC1, labels);

    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);

    cout << "trainingdatamat" << endl;
    cout << trainingDataMat << endl << endl;

    cout << "labelsMat" << endl;
    cout << labelsMat << endl << endl;

    float pre[3] = {0.09, 0.00, 0.01};
    Mat mat_pre = Mat(1, 3, CV_32F, pre);

    float res = svm->predict(mat_pre);
    cout << res;



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
    Prob1b();
    return 0;
}
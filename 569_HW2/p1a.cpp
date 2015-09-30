#include "hw1_helper.h"
using namespace cv;
using namespace std;
int main(){

    Mat im = imread("grass_01.raw");
    if (im.empty()){

        cout << "Cannot load image!" << endl;
        return -1;

    }

    imshow("Image", im);
    waitKey(0);

}
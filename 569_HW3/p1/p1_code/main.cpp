#include "Image.h"

const float PI = 3.1415926f;
const float DEGREE = 180.0f;

using namespace std;

float COS(float degree) {
    return cos(degree / DEGREE * PI);
}
float SIN(float degree) {
    return sin(degree / DEGREE * PI);
}
float TAN (float degree) {
    return tan(degree / DEGREE * PI);
}


void Test() {
    ImgPixel img[18] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    Image img_ori(3, 2, 3, &img[0]);
    img_ori.Print();
    //Image img2 (512, 512, 3, "p1_image/lenna.raw");
    //img2.Print();
    //img2.Write(&img2, "test.raw");

}

void prob1a () {

    Test();
    Image img_ori(512, 512, 3, "p1_image/lenna.raw");


}




int main() {
    cout << "Hello, World!" << endl;
    prob1a();
    return 0;
}
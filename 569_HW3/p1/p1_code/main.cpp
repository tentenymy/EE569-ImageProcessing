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
    ImgPixel img[18] = {};
    for (int i = 0; i < 36; i++) {
        img[i] = i;
    }
    Image img_ori(3, 4, 3, &img[0]);
    img_ori.Print_Data();

    img_ori.Initial_Geodata();
    img_ori.Convert_Cartesian_Coordinate();
    img_ori.Print_Geodata_Coord();

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
#include "Image.h"

using namespace std;



void Test() {
    ImgPixel img[48] = {};
    for (int i = 0; i < 48; i++) {
        img[i] = i;
    }
    Image img_ori(4, 4, 3, &img[0]);
    img_ori.Print_Data("");

    img_ori.Initial_Geodata();
    img_ori.Print_Geodata_Coord("cartesian");

    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col, 0.0f);
    img_ori.Print_Geodata_Coord("Translation");

    img_ori.Effect_Rotation(45.0f, Z);
    img_ori.Print_Geodata_Coord("Swirling");

    img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col, 0.0f);
    img_ori.Print_Geodata_Coord("Translation");

    img_ori.Set_Data();
    img_ori.Print_Data("Normal");

    cout << COS(90) << endl;

}

void prob1a () {
    Image img_ori(512, 512, 3, "p1_image/kate.raw");
    img_ori.Initial_Geodata();

    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col, 0.0f);
    img_ori.Effect_Swirling(-90.0f);
    img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col, 0.0f);

    img_ori.Set_Data();
    img_ori.Write(&img_ori, "test0.raw");
}

void prob1b1() {
    Image img_ori0(200, 200, 3, "p1_image/baby.raw");
    img_ori0.Initial_Geodata();
    img_ori0.Effect_Translation(-100.0f, -100.0f, 1.0f);
    img_ori0.Effect_Rotation(90.0f, Z);
    img_ori0.Effect_Scaling(0.01, 0.01, 1.0f);
    img_ori0.Print_Geodata_Coord("image0", 30, 4);

    Image img_ori1(200, 200, 3, "p1_image/baby_bear.raw");
    img_ori1.Initial_Geodata();
    img_ori1.Effect_Translation(-100, -100, 1);
    img_ori1.Effect_Rotation(90, Y);
    img_ori1.Effect_Rotation(90, X);
    img_ori1.Effect_Scaling(1.0f, 0.01, 0.01);
    img_ori1.Print_Geodata_Coord("image1", 30, 4);

    Image img_ori2(200, 200, 3, "p1_image/baby_cat.raw");
    img_ori2.Initial_Geodata();
    img_ori2.Effect_Translation(-100, -100, 1);
    img_ori2.Effect_Rotation(-90, X);
    img_ori2.Effect_Rotation(180, Y);
    img_ori2.Effect_Scaling(0.01, 1.0f, 0.01);
    img_ori2.Print_Geodata_Coord("image2", 30, 4);
}

void prob1b2() {
}



int main() {
    cout << "Hello, World!" << endl;
    Test();
    //prob1a();
    //prob1b1();
    return 0;
}
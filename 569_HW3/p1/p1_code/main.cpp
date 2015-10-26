#include "Image.h"

using namespace std;



void Test() {
    ImgPixel img[48] = {};
    for (int i = 0; i < 48; i++) {
        img[i] = i;
    }
    Image img_ori(4, 4, 3, &img[0]);
    //Image img_ori(512, 512, 3, "p1_image/lenna.raw");


    img_ori.Print_Data("");
    img_ori.Write(&img_ori, "test.raw");
    img_ori.Initial_Geodata();
    img_ori.Print_Geodata_Coord("Initial");
    img_ori.Image_to_Cartesian_Coordinate();
    img_ori.Print_Geodata_Coord("cartesian");

    img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col);
    img_ori.Print_Geodata_Coord("Translation");

    img_ori.Effect_Rotation(45.0f);
    img_ori.Print_Geodata_Coord("Swirling");

    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col);
    img_ori.Print_Geodata_Coord("Translation");

    img_ori.Cartesian_to_Image_Coordinate();
    img_ori.Print_Geodata_Coord("image");


    //img_ori.Normal_Coordinate();
    img_ori.Print_Geodata_Coord("Normal");

    img_ori.Set_Data();
    img_ori.Print_Data("Normal");

}

void prob1a () {
    //Test();
    Image img_ori(512, 512, 3, "p1_image/kate.raw");

    img_ori.Initial_Geodata();
    img_ori.Image_to_Cartesian_Coordinate();

    //img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col);
    //img_ori.Effect_Scaling(0.5, 0.5);
    //img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col);


    /*img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col);
    img_ori.Effect_Rotation(30);
    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col);*/

    img_ori.Effect_Translation(0.5f * img_ori.row, 0.5f * img_ori.col);
    img_ori.Effect_Swirling(90.0f);
    img_ori.Effect_Translation(-0.5f * img_ori.row, -0.5f * img_ori.col);

    img_ori.Cartesian_to_Image_Coordinate();


    img_ori.Set_Data();
    //img_ori.Print_Geodata_Coord("Normal");
    //img_ori.Print_Data("Normal");
    img_ori.Write(&img_ori, "test0.raw");
}




int main() {
    cout << "Hello, World!" << endl;
    //Test();
    prob1a();
    return 0;
}
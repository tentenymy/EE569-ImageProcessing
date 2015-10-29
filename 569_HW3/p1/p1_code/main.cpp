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


void Forward_Mapping(int density) {
    // Read image data
    vector<Image*> image_list;

    Image img_ori0(200, 200, 3, "p1_image/baby.raw");
    img_ori0.Initial_Geodata();
    img_ori0.Effect_Translation(-100.0f, -100.0f, 1.0f);
    img_ori0.Effect_Rotation(90.0f, Z);
    img_ori0.Effect_Scaling(0.01, 0.01, 1.0f);

    Image img_ori1(200, 200, 3, "p1_image/baby_bear.raw");
    img_ori1.Initial_Geodata();
    img_ori1.Effect_Translation(-100, -100, 1);
    img_ori1.Effect_Rotation(90, Y);
    img_ori1.Effect_Rotation(90, X);
    img_ori1.Effect_Scaling(1.0f, 0.01, 0.01);

    Image img_ori2(200, 200, 3, "p1_image/baby_cat.raw");
    img_ori2.Initial_Geodata();
    img_ori2.Effect_Translation(-100, -100, 1);
    img_ori2.Effect_Rotation(-90, X);
    img_ori2.Effect_Rotation(180, Y);
    img_ori2.Effect_Scaling(0.01, 1.0f, 0.01);

    image_list.push_back(&img_ori0);
    image_list.push_back(&img_ori1);
    image_list.push_back(&img_ori2);

    // Set to new image
    Image img(200, 200, 3, (ImgPixel *)NULL);
    img.Initial_Geodata(image_list);

    // Extrinsic Camera
    Coord3D coord_xc = {-1.0f, 1.0f, 0.0f};
    Coord3D coord_yc = {1.0f, 1.0f, -2.0f};
    Coord3D coord_zc = {-1.0f, -1.0f, -1.0f};
    Coord3D coord_r = {5.0f, 5.0f, 5.0f};
    img.Camera_Extrinsic(coord_xc, coord_yc, coord_zc, coord_r);

    // Intrinsic Camera
    float coef_f = (float)sqrt(3.0);
    img.Camera_Intrinsic(coef_f, 2.0f, 2.0f);

    // Display
    img.Set_Camera_Data(density);
    string filename = "cube" + to_string(density) + ".raw";
    img.Write(&img, filename);
}

void Reverse_Mapping(int density) {
    // Read image data
    vector<Image*> image_list;

    Image img_ori0(200, 200, 3, "p1_image/baby.raw");
    img_ori0.Initial_Geodata();
    img_ori0.Effect_Translation(-100.0f, -100.0f, 1.0f);
    img_ori0.Effect_Rotation(90.0f, Z);
    img_ori0.Effect_Scaling(0.01, 0.01, 1.0f);

    Image img_ori1(200, 200, 3, "p1_image/baby_bear.raw");
    img_ori1.Initial_Geodata();
    img_ori1.Effect_Translation(-100, -100, 1);
    img_ori1.Effect_Rotation(90, Y);
    img_ori1.Effect_Rotation(90, X);
    img_ori1.Effect_Scaling(1.0f, 0.01, 0.01);

    Image img_ori2(200, 200, 3, "p1_image/baby_cat.raw");
    img_ori2.Initial_Geodata();
    img_ori2.Effect_Translation(-100, -100, 1);
    img_ori2.Effect_Rotation(-90, X);
    img_ori2.Effect_Rotation(180, Y);
    img_ori2.Effect_Scaling(0.01, 1.0f, 0.01);

    image_list.push_back(&img_ori0);
    image_list.push_back(&img_ori1);
    image_list.push_back(&img_ori2);

    // Set to new image
    Image img(200, 200, 3, (ImgPixel *)NULL);
    img.Initial_Geodata(image_list);

    // Extrinsic Camera
    Coord3D coord_xc = {-1.0f, 1.0f, 0.0f};
    Coord3D coord_yc = {1.0f, 1.0f, -2.0f};
    Coord3D coord_zc = {-1.0f, -1.0f, -1.0f};
    Coord3D coord_r = {5.0f, 5.0f, 5.0f};
    img.Camera_Extrinsic(coord_xc, coord_yc, coord_zc, coord_r);

    // Intrinsic Camera
    float coef_f = (float)sqrt(3.0);
    img.Camera_Intrinsic(coef_f, 2.0f, 2.0f);

    // Reverse Mapping
    Image img_reverse(200, 200, 3, (ImgPixel*) NULL);
    img_reverse.Initial_Geodata(img.geo_data, img.geo_row, img.geo_col);
    img_reverse.Print_Geodata_Coord("reverse", 20, 2);

    // Reverse Intrinsic Camera
    img_reverse.Camera_Intrinsic_Reverse(coef_f, 2.0f, 2.0f);
    img_reverse.Print_Geodata_Coord("intrinsic", 20, 2);

    img_reverse.Camera_Extrinsic_Reverse(coord_xc, coord_yc, coord_zc, coord_r);
    img_reverse.Print_Geodata_Coord("Extrinsic", 20, 2);
    ;

    img_reverse.Set_Camera_Data_Reverse();
    //img_reverse.Print_Geodata_Coord("Image", 20, 2);



    //img_reverse.Print_Data("final");
    img_reverse.Write(&img_reverse, "test0.raw");

}


void prob1b() {
    /*Forward_Mapping(300);
    Forward_Mapping(250);
    Forward_Mapping(200);
    Forward_Mapping(150);
    Forward_Mapping(100);
    Forward_Mapping(50);*/

    Reverse_Mapping(100);


}





int main() {
    cout << "Hello, World!" << endl;
    //prob1a();
    prob1b();
    return 0;
}
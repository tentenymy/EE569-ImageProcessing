//
// Created by Maggie on 10/23/15.
//

#include "Image.h"

float COS(float degree) {
    float res;
    res = cos(degree * PI / 180.0);
    return res;
}
float SIN(float degree) {
    return sin(degree * PI / 180.0);
}
float TAN (float degree) {
    return tan(degree * PI / 180.0);
}


////////////////////////////////////
/////////// Contructor /////////////
////////////////////////////////////

Image::Image(int new_row, int new_col, int new_byte, ImgPixel *pt_img) {
    if (new_col <= 0 || new_col > 1024) {
        cerr << "wrong column" << endl;
        exit(1);
    }
    if (new_row <= 0 || new_row > 1024) {
        cerr << "wrong column" << endl;
        exit(1);
    }
    col = new_col;
    row = new_row;
    byte = new_byte;
    data = new ImgPixel [col * row * byte];
    if (!data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }
    if (pt_img == NULL) {
        for (int i = 0; i < col * row * byte; i++) {
            data[i] = 0;
        }

    } else {
        for (int i = 0; i < col * row * byte; i++) {
            data[i] = *pt_img++;
        }
    }

}

Image::Image(int new_row, int new_col, int new_byte, string filename) {
    if (new_col <= 0 || new_col > 1024) {
        cerr << "Wrong column" << endl;
        exit(1);
    }
    if (new_row <= 0 || new_row > 1024) {
        cerr << "Wrong row" << endl;
        exit(1);
    }
    col = new_col;
    row = new_row;
    byte = new_byte;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    FILE *file;
    if (!(file = fopen(c_filename, "rb")))  {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    data = new ImgPixel[col * row * byte];
    if (!data) {
        cerr << "Wrong allocate memory" << endl;
        exit(1);
    }
    fread(data, sizeof(ImgPixel), col * row * byte, file);
    fclose(file);
    if (c_filename)
        delete c_filename;
}

Image::~Image() {
    if(data)
        delete data;
    if (geo_data)
        delete geo_data;
}

/////////////////////////////////
/////////// Display /////////////
/////////////////////////////////

void Image::Write (Image *image, string filename) {
    cout << "Write " << filename << endl;
    char *c_filename = new char[filename.length() + 1];
    strcpy(c_filename, filename.c_str());
    FILE *file;
    if (!(file = fopen(c_filename, "wb"))) {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    size_t size = image->col * image->row * image->byte;
    fwrite(image->data, sizeof(ImgPixel), col * row * byte, file);
    fclose(file);
}

void Image::Print_Data(string str) {
    cout << endl << "Image " << str << ": " << row << ", " << col << ", " << byte << endl;
    int new_col = col;
    int max_col = 100;
    if (col * byte > max_col) {
        new_col = max_col / byte;
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < new_col; j++) {
            for (int k = 0; k < byte; k++) {
                cout << (int)Get_Value(i, j, k) << " ";
            }
        }
        cout << endl;
    }
}

void Image::Print_Geodata_Color(string str) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (geo_data) {
        for (int i = 0; i < geo_row; i++) {
            for (int j = 0; j < geo_col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << " "
                << geo_data[i * geo_col + j].color[0] << " "
                << geo_data[i * geo_col + j].color[1] << " "
                << geo_data[i * geo_col + j].color[2] << ") ";
            }
            cout << endl;
        }
    }
}

void Image::Print_Geodata_Color(string str, int new_row, int new_col) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (new_row > geo_row)
        new_row = geo_row;
    if (new_col > geo_col)
        new_col = geo_col;
    if (geo_data) {
        for (int i = 0; i < new_row; i++) {
            for (int j = 0; j < new_col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << " "
                << geo_data[i * geo_col + j].color[0] << " "
                << geo_data[i * geo_col + j].color[1] << " "
                << geo_data[i * geo_col + j].color[2] << ") ";
            }
            cout << endl;
        }
    }
}

void Image::Print_Geodata_Coord(string str) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (geo_data) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << ") ";
            }
            cout << endl;
        }
    }
}

void Image::Print_Geodata_Coord(string str, int new_row, int new_col) {
    cout << endl << "Image " << str << ": " << geo_row << ", " << geo_col << endl;
    if (new_row > geo_row)
        new_row = geo_row;
    if (new_col > geo_col)
        new_col = geo_col;
    if (geo_data) {
        for (int i = 0; i < new_row; i++) {
            for (int j = 0; j < new_col; j++) {
                cout << "(" << geo_data[i * geo_col + j].coord[0] << " "
                << geo_data[i * geo_col + j].coord[1] << " "
                << geo_data[i * geo_col + j].coord[2] << " "
                << geo_data[i * geo_col + j].coord[3] << ") ";
            }
            cout << endl;
        }
    }
}

ImgPixel Image::Get_Value(int i, int j, int k) {
    return data[i * byte * col + j * byte + k];
}

float Image::Get_Geo_Value(int i, int j, int k) {
    if (i < geo_row && j < geo_col && k < 3)
        return geo_data[i * geo_col + j].color[k];
    else
        return (ImgPixel)0;
}

float Image::Get_Geo_Coord(int i, int j, int k) {
    if (i < geo_row && j < geo_col && k < 4)
        return geo_data[i * geo_col + j].coord[k];
    else
        return (ImgPixel)0;
}

int Image::Get_Row(int index) {
    return index / (col * byte);
}

int Image::Get_Col(int index) {
    return (index % (byte * col)) / byte;
}

int Image::Get_Byte(int index) {
    return index % byte;
}


//////////////////////////////////
/////////// Geometry /////////////
//////////////////////////////////

int Image::Initial_Geodata() {
    cout << "Initial Geodata" << endl;
    // allocate memory to geo_data
    if (geo_data) {
        cerr << "geo data already exist" << endl;
        return 0;
    }
    geo_row = row;
    geo_col = col;
    geo_data = new GeoPixel[geo_row * geo_col];
    if (!geo_data) {
        cerr << "Wrong allocate memory" << endl;
        return 0;
    }

    // image coordinate
    for (int i = 0; i < geo_row; i++) {
        for (int j = 0; j < geo_col; j++) {
            geo_data[i * geo_col + j].coord[0] = i;
            geo_data[i * geo_col + j].coord[1] = j;
            geo_data[i * geo_col + j].coord[2] = 0.0f;
            geo_data[i * geo_col + j].coord[3] = 1.0f;
            geo_data[i * geo_col + j].color[0] = Get_Value(i, j, 0);
            geo_data[i * geo_col + j].color[1] = Get_Value(i, j, 1);
            geo_data[i * geo_col + j].color[2] = Get_Value(i, j, 2);
        }
    }
    return Image_to_Cartesian_Coordinate();
}

int Image::Initial_Geodata(vector <Image*> image_list) {
    // resize geo_col, geo row
    int image_size = image_list.size();
    int new_geo_row = 0, new_geo_col = 0;
    for (int i = 0; i < image_size; i++){
        new_geo_row += image_list[i]->geo_row;
        if (new_geo_col < image_list[i]->geo_col)
            new_geo_col = image_list[i]->geo_col;
    }
    geo_row = new_geo_row;
    geo_col = new_geo_col;
    cout << "Initial Geodata " << new_geo_row << " " << new_geo_col << endl;
    // allocate memory to geo_data
    if (geo_data) {
        cerr << "geo data already exist" << endl;
        return 0;
    }
    if (new_geo_row <= 0 && new_geo_col <= 0) {
        cerr << "geo_row or geo_col is wrong" << endl;
        return 0;
    }
    geo_data = new GeoPixel[geo_row * geo_col];
    if (!geo_data) {
        cerr << "Wrong allocate memory" << endl;
        return 0;
    }
    // image coordinate
    int image_count = 0;
    int image_row = 0;
    for (int i = 0; i < geo_row; i++) {
        for (int j = 0; j < geo_col; j++) {
            geo_data[i * geo_col + j].coord[0] = image_list[image_count]->Get_Geo_Coord(image_row, j, 0);
            geo_data[i * geo_col + j].coord[1] = image_list[image_count]->Get_Geo_Coord(image_row, j, 1);
            geo_data[i * geo_col + j].coord[2] = image_list[image_count]->Get_Geo_Coord(image_row, j, 2);
            geo_data[i * geo_col + j].coord[3] = image_list[image_count]->Get_Geo_Coord(image_row, j, 3);
            geo_data[i * geo_col + j].color[0] = image_list[image_count]->Get_Geo_Value(image_row, j, 0);
            geo_data[i * geo_col + j].color[1] = image_list[image_count]->Get_Geo_Value(image_row, j, 1);
            geo_data[i * geo_col + j].color[2] = image_list[image_count]->Get_Geo_Value(image_row, j, 2);
        }
        image_row++;
        if (image_row >= image_list[image_count]->geo_row) {
            image_count++;
            image_row = 0;
        }
    }
    return 1;
}

int Image::Initial_Geodata(GeoPixel* new_geo_data, int new_geo_row, int new_geo_col) {
    // resize geo_col, geo row
    geo_row = new_geo_row;
    geo_col = new_geo_col;
    cout << "Initial Geodata " << new_geo_row << " " << new_geo_col << endl;
    // allocate memory to geo_data
    if (geo_data) {
        cerr << "geo data already exist" << endl;
        return 0;
    }
    if (new_geo_row <= 0 && new_geo_col <= 0) {
        cerr << "geo_row or geo_col is wrong" << endl;
        return 0;
    }
    geo_data = new GeoPixel[geo_row * geo_col];
    if (!geo_data) {
        cerr << "Wrong allocate memory" << endl;
        return 0;
    }
    // image coordinate
    int image_count = 0;
    int image_row = 0;
    for (int i = 0; i < geo_row; i++) {
        for (int j = 0; j < geo_col; j++) {
            geo_data[i * geo_col + j].coord[0] = new_geo_data[i * geo_col + j].coord[0];
            geo_data[i * geo_col + j].coord[1] = new_geo_data[i * geo_col + j].coord[1];
            geo_data[i * geo_col + j].coord[2] = new_geo_data[i * geo_col + j].coord[2];
            geo_data[i * geo_col + j].coord[3] = new_geo_data[i * geo_col + j].coord[3];
            geo_data[i * geo_col + j].color[0] = new_geo_data[i * geo_col + j].color[0];
            geo_data[i * geo_col + j].color[1] = new_geo_data[i * geo_col + j].color[1];
            geo_data[i * geo_col + j].color[2] = new_geo_data[i * geo_col + j].color[2];
        }
    }
    return 1;
}


int Image::Image_to_Cartesian_Coordinate() {
    cout << "Convert_Cartesian_Coordinate" << endl;
    float temp_row = (float)row - 0.5f;
    Matrix matrix = {{0.0f, 1.0f, 0.0f, 0.5f},
                    {-1.0f, 0.0f, 0.0f, temp_row},
                    {0.0f, 0.0f, 1.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 1.0f}};
    return Apply_Matrix(matrix);
}




int Image::Cartesian_to_Image_Coordinate() {
    cout << "Convert_Image_Coordinate" << endl;
    float temp_row = (float)row - 0.5f;
    Matrix matrix = {{0.0f, -1.0f, 0.0f, temp_row},
                    {1.0f, 0.0f, 0.0f, -0.5f},
                    {0.0f, 0.0f, 1.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 1.0f}};
    return Apply_Matrix(matrix);
}


int Image::Apply_Matrix(Matrix matrix) {
    // check geo_data
    if (!geo_data) {
        cerr << "geo data is still null" << endl;
        return 0;
    }
    // apply matrix to Image coordinate
    ImgCoord temp_coord = {};
    for(int i = 0; i < geo_row * geo_col; i++) {
        for (int j = 0; j < 4; j++) {
            temp_coord[j] = 0;
            for (int k = 0; k < 4; k++) {
                temp_coord[j] += matrix[j][k] * geo_data[i].coord[k];
            }
        }
        geo_data[i].coord[0] = temp_coord[0];
        geo_data[i].coord[1] = temp_coord[1];
        geo_data[i].coord[2] = temp_coord[2];
        geo_data[i].coord[3] = temp_coord[3];
    }
    return 1;
}


int Image::Set_Data() {
    cout << "Set Data" << endl;
    // check geo_data
    if (!geo_data) {
        cerr << "geo data is still null" << endl;
        return 0;
    }
    // convert geodata to image coordinate from Cartesian
    if (Cartesian_to_Image_Coordinate() == 0)
        return 0;

    // Set temp data
    float *temp_data = new float[row * col * byte];
    if (!temp_data) {
        cerr << "fail to allocate memory" << endl;
        return 0;
    }
    for (int i = 0; i < row * col * byte; i++) {
        temp_data[i] = 0.0f;
    }
    // bilinear intepolation
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            float point_estimate_x = geo_data[i * geo_col + j].coord[0];
            float point_estimate_y = geo_data[i * geo_col + j].coord[1];
            int point_lt_x = (int)floor(point_estimate_x);
            int point_lt_y = (int)floor(point_estimate_y);
            float scale_a = point_estimate_x - (float)point_lt_x;
            float scale_b = point_estimate_y - (float)point_lt_y;

            int point_x[4] = {point_lt_x, point_lt_x, point_lt_x + 1, point_lt_x + 1};
            int point_y[4] = {point_lt_y, point_lt_y + 1, point_lt_y, point_lt_y + 1};
            float scale_ab[4] = {(1.0f - scale_a) * (1.0f - scale_b), (1.0f - scale_a) * scale_b, (1.0f - scale_b) * scale_a, scale_a * scale_b};
            for(int k = 0; k < byte; k++)
            {
                float value = 0;
                for (int m = 0; m < 4; m++) {
                    if (point_x[m] >= 0 && point_x[m] <= row && point_y[m] >= 0 && point_y[m] <= col)
                        value += scale_ab[m] * Get_Geo_Value(point_x[m], point_y[m], k);
                }
                temp_data[i * col * byte + j * byte + k] = value;
            }
        }
    }
    for (int i = 0; i < row * col * byte; i++) {
        data[i] = (ImgPixel)temp_data[i];
    }
    delete temp_data;
    return 1;
}



////////////////////////////////
/////////// Effect /////////////
////////////////////////////////

int Image::Effect_Rotation(float theta, int mode) {
    cout << "Effect_Rotation: " << theta << " in " << mode << "axie" << endl;
    if (mode == X) {
        Matrix matrix1 = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, COS(theta), -SIN(theta), 0.0f},
                          {0.0f, SIN(theta), COS(theta), 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
        return Apply_Matrix(matrix1);
    } else if (mode == Y) {
        Matrix matrix2 = {{COS(theta), 0.0f, SIN(theta), 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
                          {-SIN(theta), 0.0f, COS(theta), 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
        return Apply_Matrix(matrix2);
    } else if (mode == Z) {
        Matrix matrix3 = {{COS(theta), -SIN(theta), 0.0f, 0.0f}, {SIN(theta), COS(theta), 0.0f, 0.0f},
                          {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
        return Apply_Matrix(matrix3);
    } else {
        cout << "Wrong mode " << mode << endl;
        return 0;
    }

}


int Image::Effect_Translation(float coef_tx, float coef_ty, float coef_tz) {
    cout << "Effect_Translation: " << coef_tx << " " << coef_ty << " " << coef_tz << endl;
    Matrix matrix = {{1.0f, 0.0f, 0.0f, coef_tx}, {0.0f, 1.0f, 0.0f, coef_ty},
                    {0.0f, 0.0f, 1.0f, coef_tz}, {0.0f, 0.0f, 0.0f, 1.0f}};
    return Apply_Matrix(matrix);
}

int Image::Effect_Scaling(float coef_sx, float coef_sy, float coef_sz) {
    cout << "Effect_Scaling: " << coef_sx << " " << coef_sy << " " << coef_sz << endl;
    Matrix matrix = {{coef_sx, 0.0f, 0.0f, 0.0f}, {0.0f, coef_sy, 0.0f, 0.0f},
                     {0.0f, 0.0f, coef_sz, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
    return Apply_Matrix(matrix);
}


int Image::Effect_Swirling(float theta) {
    cout << "Effect_Swirling: " << theta << endl;
    // check geo_data
    if (!geo_data) {
        cerr << "geo data is still null" << endl;
        return 0;
    }
    // apply matrix to Image coordinate
    ImgCoord temp_coord = {};
    for(int i = 0; i < geo_row * geo_col; i++) {
        float range = geo_data[i].coord[0] * geo_data[i].coord[0] + geo_data[i].coord[1] * geo_data[i].coord[1];
        float theta_current = -theta * range * 4 / (row * col);
        Matrix matrix = {{COS(theta_current), -SIN(theta_current), 0.0f, 0.0f}, {SIN(theta_current), COS(theta_current), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
        for (int j = 0; j < 4; j++) {
            temp_coord[j] = 0;
            for (int k = 0; k < 4; k++) {
                temp_coord[j] += matrix[j][k] * geo_data[i].coord[k];
            }
        }
        geo_data[i].coord[0] = temp_coord[0];
        geo_data[i].coord[1] = temp_coord[1];
        geo_data[i].coord[2] = temp_coord[2];
        geo_data[i].coord[3] = temp_coord[3];
    }
    return 1;
}

///////////////////////////////////
/////////// 3D camera /////////////
///////////////////////////////////

int Image::Camera_Extrinsic(Coord3D coord_xc, Coord3D coord_yc, Coord3D coord_zc, Coord3D coord_r) {
    // normalize coordinate
    double scale_xc = 1.0 / sqrt(coord_xc[0] * coord_xc[0] + coord_xc[1] * coord_xc[1] + coord_xc[2] * coord_xc[2]);
    double scale_yc = 1.0 / sqrt(coord_yc[0] * coord_yc[0] + coord_yc[1] * coord_yc[1] + coord_yc[2] * coord_yc[2]);
    double scale_zc = 1.0 / sqrt(coord_zc[0] * coord_zc[0] + coord_zc[1] * coord_zc[1] + coord_zc[2] * coord_zc[2]);
    for (int i = 0; i < 3; i++) {
        coord_xc[i] *= scale_xc;
        coord_yc[i] *= scale_yc;
        coord_zc[i] *= scale_zc;
    }
    // Set matrix
    Matrix matrix =
    {{coord_xc[0], coord_xc[1], coord_xc[2], -1.0f * (coord_r[0] * coord_xc[0] + coord_r[1] * coord_xc[1] + coord_r[2] * coord_xc[2])},
    {coord_yc[0], coord_yc[1], coord_yc[2], -1.0f * (coord_r[0] * coord_yc[0] + coord_r[1] * coord_yc[1] + coord_r[2] * coord_yc[2])},
    {coord_zc[0], coord_zc[1], coord_zc[2], -1.0f * (coord_r[0] * coord_zc[0] + coord_r[1] * coord_zc[1] + coord_r[2] * coord_zc[2])},
    {0.0f, 0.0f, 0.0f, 1.0f}};
    return Apply_Matrix(matrix);
}

int Image::Camera_Intrinsic(float coef_f, float coef_cx, float coef_cy) {
    Matrix matrix = {{coef_f, 0.0f, coef_cx / 2.0f, 0.0f}, {0.0f, coef_f, coef_cy / 2.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
    Apply_Matrix(matrix);
    for (int i = 0; i < geo_row * geo_col; i++) {
        geo_data[i].coord[0] /= geo_data[i].coord[2];
        geo_data[i].coord[1] /= geo_data[i].coord[2];
    }
    return 1;
}

int Image::Camera_Intrinsic_Reverse(float coef_f, float coef_cx, float coef_cy) {
    for (int i = 0; i < geo_row * geo_col; i++) {
        geo_data[i].coord[0] *= geo_data[i].coord[2];
        geo_data[i].coord[1] *= geo_data[i].coord[2];
    }
    Matrix matrix = {{1.0f / coef_f, 0.0f, -coef_cx / (2.0f * coef_f), 0.0f},
                     {0.0f, 1.0f / coef_f, -coef_cy / (2.0f * coef_f), 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
    Apply_Matrix(matrix);
    return 1;
}

int Image::Camera_Extrinsic_Reverse(Coord3D coord_xc, Coord3D coord_yc, Coord3D coord_zc, Coord3D coord_r) {
    // normalize coordinate
    double scale_xc = 1.0 / sqrt(coord_xc[0] * coord_xc[0] + coord_xc[1] * coord_xc[1] + coord_xc[2] * coord_xc[2]);
    double scale_yc = 1.0 / sqrt(coord_yc[0] * coord_yc[0] + coord_yc[1] * coord_yc[1] + coord_yc[2] * coord_yc[2]);
    double scale_zc = 1.0 / sqrt(coord_zc[0] * coord_zc[0] + coord_zc[1] * coord_zc[1] + coord_zc[2] * coord_zc[2]);
    for (int i = 0; i < 3; i++) {
        coord_xc[i] *= scale_xc;
        coord_yc[i] *= scale_yc;
        coord_zc[i] *= scale_zc;
    }
    // Set matrix
    Matrix matrix =
            {{coord_xc[0], coord_yc[0], coord_zc[0], coord_r[0]},
             {coord_xc[1], coord_yc[1], coord_zc[1], coord_r[1]},
             {coord_xc[2], coord_yc[2], coord_zc[2], coord_r[2]},
             {0.0f, 0.0f, 0.0f, 1.0f}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    return Apply_Matrix(matrix);
}

int Image::Set_Camera_Data(int density) {
    cout << "Set Data" << endl;
    // check geo_data
    if (!geo_data) {
        cerr << "geo data is still null" << endl;
        return 0;
    }

    float max_x = -10000;
    float max_y = -10000;
    float min_x = 10000;
    float min_y = 10000;
    for (int i = 0; i < geo_row * geo_col; i++) {
        geo_data[i].coord[0] *= density;
        geo_data[i].coord[1] *= density;
        if (max_x < geo_data[i].coord[0])
            max_x = geo_data[i].coord[0];
        if (max_y < geo_data[i].coord[1])
            max_y = geo_data[i].coord[1];
        if (min_x > geo_data[i].coord[0])
            min_x = geo_data[i].coord[0];
        if (min_y > geo_data[i].coord[1])
            min_y = geo_data[i].coord[1];
    }
    Effect_Translation(100 - density, 100 - density, 0);


    for (int i = 0; i < geo_row; i++) {
        for (int j = 0; j < geo_col; j++) {
            int temp_i = (int)geo_data[i * geo_col + j].coord[0];
            int temp_j = (int)geo_data[i * geo_col + j].coord[1];
            int index =  temp_i * col * byte + temp_j * byte;
            if (index >= 0 && index < col * byte * row - 2) {
                data[index] = (ImgPixel)Get_Geo_Value(i, j, 0);
                data[index + 1] = (ImgPixel)Get_Geo_Value(i, j, 1);
                data[index + 2] = (ImgPixel)Get_Geo_Value(i, j, 2);
            }
        }
    }
    return 1;
}

int Image::Set_Camera_Data_Reverse() {
    cout << "Set Data" << endl;
    // check geo_data
    if (!geo_data) {
        cerr << "geo data is still null" << endl;
        return 0;
    }

    float max_x = -10000;
    float max_y = -10000;
    float min_x = 10000;
    float min_y = 10000;
    int density = 50;
    for (int i = 0; i < geo_row * geo_col; i++) {
        geo_data[i].coord[0] *= 50;
        geo_data[i].coord[1] *= 50;
        if (max_x < geo_data[i].coord[0])
            max_x = geo_data[i].coord[0];
        if (max_y < geo_data[i].coord[1])
            max_y = geo_data[i].coord[1];
        if (min_x > geo_data[i].coord[0])
            min_x = geo_data[i].coord[0];
        if (min_y > geo_data[i].coord[1])
            min_y = geo_data[i].coord[1];
    }
    Effect_Translation(50, 50, 0);
    Cartesian_to_Image_Coordinate();
    Print_Geodata_Coord("Image2", 20, 2);
    Effect_Translation(-50, 50, 0);

    float z_buffer = INT_MAX;
    for (int i = 0; i < geo_row; i++) {
        for (int j = 0; j < geo_col; j++) {
            int temp_i = (int)geo_data[i * geo_col + j].coord[0];
            int temp_j = (int)geo_data[i * geo_col + j].coord[1];
            int index =  temp_i * col * byte + temp_j * byte;
            if (index >= 0 && index < col * byte * row - 2) {
                data[index] = (ImgPixel)Get_Geo_Value(i, j, 0);
                data[index + 1] = (ImgPixel)Get_Geo_Value(i, j, 1);
                data[index + 2] = (ImgPixel)Get_Geo_Value(i, j, 2);
                z_buffer = Get_Geo_Coord(i, j, 2);
            }
        }
    }
    return 1;
}


// EE569 Homework Assignment #1
// Date:     Sep 20, 2015
// Name:     Meiyi Yang
// ID:       6761-0405-85
// Email:    meiyiyan@usc.edu

#include "hw1_helper.h"

using namespace std;

Info::Info()
{
    file = NULL;
    filename_read = "the_starry_night.raw";
    filename_write = "output_p1a.raw";
    byteperpixel = COLOR_BYTE;
    width = DEFAULT_SIZE;
    height = DEFAULT_SIZE;
}


Info::Info(int argc, char *argv[])
{
    // Set value
    file = NULL;
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Width 512] [Height = 512]" << endl;
        exit(1);
    }
    else if (argc < 4)
    {
        filename_read = argv[1];
        filename_write = argv[2];
        byteperpixel = COLOR_BYTE;
        width = DEFAULT_SIZE;
        height = DEFAULT_SIZE;
    }
    else if (argc < 5)
    {
        filename_read = argv[1];
        filename_write = argv[2];
        byteperpixel = atoi(argv[3]);
        width = DEFAULT_SIZE;
        height = DEFAULT_SIZE;
    }
    else if (argc < 6)
    {
        filename_read = argv[1];
        filename_write = argv[2];
        byteperpixel = atoi(argv[3]);
        width = atoi(argv[4]);
        height = width;
    }
    else if (argc < 7)
    {
        filename_read = argv[1];
        filename_write = argv[2];
        byteperpixel = atoi(argv[3]);
        width = atoi(argv[4]);
        height = atoi(argv[5]);
    }
    else if (argc < 8) // ONLY FOR DEBUG in CLION
    {
        filename_read = argv[2];
        filename_write = argv[3];
        byteperpixel = atoi(argv[4]);
        width = atoi(argv[5]);
        height = atoi(argv[6]);
    }
    else
    {
        cout << "Syntax Error - Too much Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Width 512] [Height = 512]" << endl;
        exit(1);
    }

    // Validation
    if (filename_read == NULL || filename_write == NULL)
    {
        cout << "Filename_read or filename_write is invalid" << endl;
        exit(1);
    }
    if (byteperpixel <= 0 || byteperpixel > MAX_BYTE)
    {
        cout << "Byte per pixel is invalid" << endl;
        exit(1);
    }
    if (width <= 0 || width > MAX_SIZE || height <= 0 || height > MAX_SIZE)
    {
        cout << "Width or height is invalid" << endl;
        exit(1);
    }
}


// print the information for me.
void Info::Info_Print()
{
    Check_Debug();
    cout << "File_read: " << filename_read << endl;
    cout << "File_write: " << filename_write << endl;
    cout << "Byte per pixel: " << byteperpixel << endl;
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;
}

// read filename_read
Image* Info::Info_File_Read()
{
    char new_filename[60];
    strcpy(new_filename, FOLDER_READ);
    strcat(new_filename, filename_read);
    if (!(file = fopen(new_filename, "rb"))) {
        cout << "Cannot open file: " << filename_read << endl;
        exit(1);
    }

    Image image[height][width][byteperpixel];
    fread(image, sizeof(Image), (size_t) width * height * byteperpixel, file);
    fclose(file);

    Image *pt_image = &image[0][0][0];
    return pt_image;
}

Image* Info::Info_File_Read(char* filename)
{
    char new_filename[60];
    strcpy(new_filename, FOLDER_READ);
    strcat(new_filename, filename);
    if (!(file = fopen(new_filename, "rb"))) {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }

    Image image[height][width][byteperpixel];
    fread(image, sizeof(Image), (size_t) width * height * byteperpixel, file);
    fclose(file);

    Image *pt_image = &image[0][0][0];
    return pt_image;
}

// write filename_write
void Info::Info_File_Write(Image *pt_image)
{
    char new_filename[60];
    strcpy(new_filename, FOLDER_WRITE);
    strcat(new_filename, filename_write);
    if (!(file = fopen(new_filename, "wb")))
    {
        cout << "Cannot open file: " << filename_write <<endl;
        exit(1);
    }

    Image image[height] [width] [byteperpixel];
    memcpy(image, pt_image, sizeof(image));
    fwrite(image, sizeof(Image), (size_t)width * height * byteperpixel, file);
    fclose(file);
}

void Info::Info_File_Write(Image *pt_image, char* filename)
{
    char new_filename[60];
    strcpy(new_filename, FOLDER_WRITE);
    strcat(new_filename, filename);
    if (!(file = fopen(new_filename, "wb")))
    {
        cout << "Cannot open file: " << filename <<endl;
        exit(1);
    }

    Image image[height] [width] [byteperpixel];
    memcpy(image, pt_image, sizeof(image));
    fwrite(image, sizeof(Image), (size_t)width * height * byteperpixel, file);
    fclose(file);
}


// Some Helper function for debug
void Image_Print_By_Interger(Image *pt_image, Info *pt_info, string filename)
{
    //Usage: Image_Print_By_Interger(&imagedata[0][0][0], &info, "image_print_by_interger.txt");
    Check_Debug();
    cout << "Image_Print_By_Interger() begin" << endl;

    filename = FOLDER_DEBUG + filename;
    ofstream fout;
    fout.open(filename);
    for(int i = 0; i < pt_info->height; i++)
    {
        fout << "ROW" << i << ": ";
        for (int j = 0; j < pt_info->width; j++)
        {
            for(int k = 0; k < pt_info->byteperpixel; k++)
            {
                fout << (int)*pt_image++ << " ";
            }
        }
        fout << endl;
        fout << endl;
    }
    fout.close();
}

void Image_Plot_Gray_Line(Image *pt_image, Info *pt_info, string filename)
{

    // Usage: Image_Plot_Gray_Line(&image_data[0][0][0], &info, "image_plot_gray_line.txt");
    Check_Debug();
    cout << "Image_Plot_Line() begin" << endl;



    // 1. Get gray line
    short grayline[pt_info->width * pt_info->height];

    for(int i = 0; i < pt_info->width; i++)
    {
        for (int j = 0; j < pt_info->height; j++)
        {
            grayline[i * pt_info->height + j] = (short)*pt_image++;
        }
    }

    // 2. Set all the parameters
    // raw data
    int size = pt_info->width * pt_info->height;

    int max = -1;
    int min = 1000;
    int total = 0;
    double average = 0;
    int color_occur_times[COLOR_SIZE] = {};
    int temp = 0;

    int color_per_group = (int)ceil(COLOR_SIZE/(double)PLOT_X_SIZE);
    int color_plot_times[PLOT_X_SIZE] = {};
    int *pt_color_occur_times = &color_occur_times[0];

    for(int i = 0; i < sizeof(grayline)/sizeof(short); i++)
    {
        temp = grayline[i];
        color_occur_times[temp]++;
        if(temp > max)
        {
            max = temp;
        }
        if(temp < min)
        {
            min = temp;
        }
        total += temp;
    }
    average = total/(double)size;
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        for (int j = 0; j < color_per_group; j++)
        {
            color_plot_times[i] += *pt_color_occur_times++;
        }
    }

    filename = FOLDER_DEBUG + filename;
    ofstream fout;
    fout.open(filename);
    fout << "Max: " << max << " Min: " << min << endl;
    fout << "Average: " << average << endl;
    fout << "Size: " << size << endl;
    fout << "PLOT: \t" << "0              " << PLOT_Y_SIZE * 14 << endl;
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        fout << i * color_per_group << ": \t";
        color_plot_times[i] = color_plot_times[i]/PLOT_Y_SIZE;
        for(int j = 0; j < color_plot_times[i]; j++)
        {
            fout << "*";
        }
        fout << endl;
    }
    fout << endl;
    fout << endl;
    fout << endl;
    fout.close();
}

void Check_Debug()
{
    // Usage: Check_Debug();
    if (DEBUG == 1)
    {
        cout << "------DEBUG------" << endl;
    }
    else
    {
        cout << "------DEBUG------" << endl;
        exit(1);
    }
}

// Re-write if have time OMG TOO LONG;
void Image_Plot_All_Line(Image *pt_image, Info *pt_info, string filename)
{
    // Usage: Image_Plot_All_Line(&image_data[0][0][0], &info, "image_plot_all_line.txt");
    Check_Debug();
    cout << "Image_Plot_All_Line() begin" << endl;

    // 1. Get each color line
    short redline[pt_info->width * pt_info->height];
    short greenline[pt_info->width * pt_info->height];
    short blueline[pt_info->width * pt_info->height];

    for(int i = 0; i < pt_info->width; i++)
    {
        for (int j = 0; j < pt_info->height; j++)
        {
            redline[i * pt_info->height + j] = (short)*pt_image++;
            greenline[i * pt_info->height + j] = (short)*pt_image++;
            blueline[i * pt_info->height + j] = (short)*pt_image++;
        }
    }

    // 2. Set all the parameters
    // raw data
    short * pt_redline = redline;
    short * pt_greenline  = greenline;
    short * pt_blueline = blueline;
    int size = pt_info->width * pt_info->height;

    // for statistics
    int max = -1;
    int min = 1000;
    int total = 0;
    double average = 0;
    int temp = 0;

    // for plot
    int redcolor_occur_times[COLOR_SIZE] = {}; // 256 groups
    int redcolor_plot_times[PLOT_X_SIZE] = {}; // turn to plot 32 groups
    int *pt_redcolor_occur_times = &redcolor_occur_times[0];
    int greencolor_occur_times[COLOR_SIZE] = {};
    int greencolor_plot_times[PLOT_X_SIZE] = {};
    int *pt_greencolor_occur_times = &greencolor_occur_times[0];
    int bluecolor_occur_times[COLOR_SIZE] = {};
    int bluecolor_plot_times[PLOT_X_SIZE] = {};
    int *pt_bluecolor_occur_times = &bluecolor_occur_times[0];
    int color_per_group = (int)ceil(COLOR_SIZE/(double)PLOT_X_SIZE);

    // for file
    filename = FOLDER_DEBUG + filename;
    ofstream fout;
    fout.open(filename);

    // 3. Red begin to calculate and write in file
    // change to times * 256 and calculate max, min, total, avg
    for(int i = 0; i < size; i++)
    {
        temp = *pt_redline++;
        redcolor_occur_times[temp]++;
        if(temp > max)
            max = temp;
        if(temp < min)
            min = temp;
        total += temp;
    }
    average = total/(double)size;

    // change to times * PLOT_X_SIZE
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        for (int j = 0; j < color_per_group; j++)
            redcolor_plot_times[i] += *pt_redcolor_occur_times++;
    }

    // write to file
    fout << "------------------------- RED -------------------------" << endl;
    fout << "Max: " << max << " Min: " << min << " Average: " << average << " Size: " << size << endl;
    fout << "Plot\t" << "0         5       10" << endl;
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        fout << i * color_per_group << ": \t";
        redcolor_plot_times[i] = redcolor_plot_times[i]/PLOT_Y_SIZE;
        for(int j = 0; j < redcolor_plot_times[i]; j++)
            fout << "*";
        fout << endl;
    }

    // 4. Green begin to calculate and write in file
    // reset the parameter
    max = -1;
    min = 1000;
    total = 0;
    average = 0;

    // change to times * 256 and calculate max, min, total, avg
    for(int i = 0; i <  size; i++)
    {
        temp = *pt_greenline++;
        greencolor_occur_times[temp]++;
        if(temp > max)
            max = temp;
        if(temp < min)
            min = temp;
        total += temp;
    }
    average = total/(double)size;

    // change to times * PLOT_X_SIZE
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        for (int j = 0; j < color_per_group; j++)
            greencolor_plot_times[i] += *pt_greencolor_occur_times++;
    }

    // write to file
    fout << "------------------------- GREEN -------------------------" << endl;
    fout << "Max: " << max << " Min: " << min << " Average: " << average << " Size: " << size << endl;
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        fout << i * color_per_group << ": \t";
        greencolor_plot_times[i] = greencolor_plot_times[i]/PLOT_Y_SIZE;
        for(int j = 0; j < greencolor_plot_times[i]; j++)
            fout << "*";
        fout << endl;
    }

    // 5. Blue begin to calculate and write in file
    // reset the parameter
    max = -1;
    min = 1000;
    total = 0;
    average = 0;

    // change to times * 256 and calculate max, min, total, avg
    for(int i = 0; i <  size; i++)
    {
        temp = *pt_blueline++;
        bluecolor_occur_times[temp]++;
        if(temp > max)
            max = temp;
        if(temp < min)
            min = temp;
        total += temp;
    }
    average = total/(double)size;

    // change to times * PLOT_X_SIZE
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        for (int j = 0; j < color_per_group; j++)
            bluecolor_plot_times[i] += *pt_bluecolor_occur_times++;
    }

    // write to file
    fout << "------------------------- BLUE -------------------------" << endl;
    fout << "Max: " << max << " Min: " << min << " Average: " << average << " Size: " << size << endl;
    for(int i = 0; i < PLOT_X_SIZE; i++)
    {
        fout << i * color_per_group << ": \t";
        bluecolor_plot_times[i] = bluecolor_plot_times[i]/PLOT_Y_SIZE;
        for(int j = 0; j < bluecolor_plot_times[i]; j++)
            fout << "*";
        fout << endl;
    }

    // 6. Close the file
    fout.close();
}




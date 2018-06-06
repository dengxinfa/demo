#include "stdio.h"

#pragma pack(2)        //两字节对齐，否则bmp_fileheader会占16Byte

struct bmp_fileheader
{
    unsigned short    bfType;        //若不对齐，这个会占4Byte
    unsigned long    bfSize;
    unsigned short    bfReverved1;
    unsigned short    bfReverved2;
    unsigned long    bfOffBits;
};

struct bmp_infoheader
{
    unsigned long    biSize;
    unsigned long    biWidth;
    unsigned long    biHeight;
    unsigned short    biPlanes;
    unsigned short    biBitCount;
    unsigned long    biCompression;
    unsigned long    biSizeImage;
    unsigned long    biXPelsPerMeter;
    unsigned long    biYpelsPerMeter;
    unsigned long    biClrUsed;
    unsigned long    biClrImportant;
};

void savebmp(char* data, int width, int height, char *suffix)
{
    static int index = 0;
    char bmp_name[64];
    int i, j;
#if 0
    LOGI("<<<<<<<<<<<<<<<<<<<");
    for (i = 0; i < height / 2; i++) {
    LOGI("<<<<<<<<<<<<<<<<<<<i:%d", i);
        for (j = 0; j < width; j++) {
    LOGI("<<<<<<<<<<<<<<<<<<<i:%d j:%d", i, j);
            uint8_t b = data[i * width * 3 + j * 3];
            uint8_t g = data[i * width * 3 + j * 3 + 1];
            uint8_t r = data[i * width * 3 + j * 3 + 2];

    //LOGI("<<<<<<<<<<<<<<<<<<<i:%d j:%d", i, j);
            int row = height - i;
    LOGI("<<<<<<<<<<<<<<<<<<<i:%d j:%d row:%d width:%d height:%d",
        i, j, row, width, height);
            data[i * width * 3 + j * 3] = data[row * width * 3 + j * 3];
            data[i * width * 3 + j * 3 + 1] = data[row * width * 3 + j * 3 + 1];
            data[i * width * 3 + j * 3 + 2] = data[row * width * 3 + j * 3 + 2];

            data[row * width * 3 + j * 3] = b;
            data[row * width * 3 + j * 3 + 1] = g;
            data[row * width * 3 + j * 3 + 2] = r;
        }
    }
#endif
    struct bmp_fileheader bfh;
    struct bmp_infoheader bih;
    //LOGI("<<<<<<<<<<<<<<<<<<<");
    if(suffix)
    {
        sprintf(bmp_name, "./fd_%s.bmp", suffix);
    }
    else
    {
        sprintf(bmp_name, "./fd_%d.bmp", index++);
    }
    printf("fopen %s, width:%d height:%d size:%d\n",
        bmp_name, width, height, 54+width*height*3);
    FILE* output_file = fopen(bmp_name,"wb");

    bfh.bfSize=54+width*height*3;
    bfh.bfOffBits=54;
    bfh.bfType = 0x4D42;
    bfh.bfReverved1 = 0;
    bfh.bfReverved2 = 0;
    fwrite((char*)&bfh,sizeof(struct bmp_fileheader),1,output_file);


    bih.biBitCount=24;            //8位灰度图
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes=1;
    bih.biCompression=0;
    bih.biSizeImage=width * height;
    bih.biXPelsPerMeter=0;
    bih.biYpelsPerMeter=0;
    bih.biClrUsed=0;
    bih.biClrImportant=0;
    bih.biSize=40;
    printf("w:%d h:%d size:%d\n", bih.biWidth, bih.biHeight, bih.biSizeImage);
    fwrite((char*)&bih,sizeof(struct bmp_infoheader),1,output_file);
    fwrite((char*)data,width * height * 3,1,output_file);
    fclose(output_file);
}


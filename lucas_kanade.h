#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct bmpFileHeader{
    unsigned char **imageArray;   // only colour pixels
    unsigned short signature; // bmp,png or ...
    unsigned int fileSize;
    unsigned int reserved1; // do not needed
    unsigned int offset; // which tells at which byte the image starts
    unsigned int sizeofDibHeader; // size of dibheader
    unsigned int width; // width of image
    int height; // height of image
    unsigned short ncp; // number of color planes
    unsigned short npp; // number per pixel
    unsigned int *restDibHeader; // rest part of header 
} FHeader;


typedef struct pixel
{
    unsigned int r;
    unsigned int b;
    unsigned int g;
}PIXEL;

typedef struct avgpixel
{
    unsigned int avgR;
    unsigned int avgB;
    unsigned int avgG;
}avgPixel;


// resolution struct will store pixel array and average colors;
typedef struct resStruct
{
    unsigned char **imageArray;
    avgPixel *avgArray;
}RES;

FHeader readFileHeader(FILE* userGivenName); // reading all the contents of file to a STRUCTURE
void printContent(FHeader image,char* fileName); // PRINTING CONTENT OF AN ARRAY
void printImageArray(FHeader image); // PRINTING ONLY CONTENTS OF AN ARRAY
void writeToFile(char *fileName,FHeader image,unsigned char **arrayStrip); // WRITING ALL THE IMPORTANT DATA(WITHOUT PIXEL DATA) + NEW PIXEL ARRAY TO A NEW FILE
unsigned char** decreasedResolutionArray(FHeader image);
avgPixel averageColor(unsigned char **mat,int width,int height,int ppx);
unsigned char** compareImages(unsigned char **imageArray1,unsigned char **imageArray2,int width,int height,int ppx,int factor,int flag);
int range(int px11,int px12,int px13,int px21,int px22,int px23);
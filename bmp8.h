#include <stdio.h>
#include <stdlib.h>
#ifndef BMP8_H
#define BMP8_H

// tmbp8 structure

typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;



// Functions to read and write images
t_bmp8* bmp8_loadImage(const char* f_name);
void bmp8_saveImage(const char* f_name, t_bmp8* img);
void bmp8_free(t_bmp8* img);
void bmp8_printInfo(t_bmp8* img);

// Image processing functions
void bmp8_negative(t_bmp8* img);
void bmp8_brightness(t_bmp8* img, int value);
void bmp8_threshold(t_bmp8* img, int threshold);

// Filter application function
void bmp8_applyFilter(t_bmp8* img, int** kernel, int kernelSize);


int box_blur[9] = {1/9,1/9,1/9,1/9,1/9,1/9,1/9};
float gaussian_blur[9] = {1/16, 1/8, 1/16, 1/4, 1/16, 1/8, 1/16};
float outline[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
float emboss[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
float sharpen[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};


unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_computeCDF(unsigned int * hist);
void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);

#endif // BMP8_H
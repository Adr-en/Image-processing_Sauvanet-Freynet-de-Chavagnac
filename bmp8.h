#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
void bmp8_applyFilter(t_bmp8* img, float ** kernel, int kernelSize);


// Histogram
unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_computeCDF(unsigned int * hist);
void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);

#endif // BMP8_H
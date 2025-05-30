/* This file is the header of the bmp24.c file
 * Here every function of the bmp24.c file is listed and te$hne can be called
 */
#ifndef BMP24_H
#define BMP24_H

#include <stdint.h> //for uint8_t, uint16_t, uint32_t
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>


//Structures
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;


//Pixel RGB
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;


//Images 24 bits
typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel** data; // Matrice de pixels
} t_bmp24;


//Functions of the file
t_bmp24* bmp24_loadImage(const char* f_name);
void bmp24_saveImage(t_bmp24* image, const char* f_name);
void bmp24_printInfo(t_bmp24* image);
void bmp24_free(t_bmp24* image);

void bmp24_negative(t_bmp24* image);
void bmp24_grayscale(t_bmp24* image);
void bmp24_brightness(t_bmp24* image, int value);

t_pixel bmp24_convolution(t_bmp24 *image, int x, int y, float **kernel, int kernelSize);
void bmp24_applyFilter(t_bmp24 *image, float **kernel, int kernelSize);
void bmp24_equalize(t_bmp24 *image);
t_pixel ** bmp24_allocateDataPixels (int width, int height);
void bmp24_freeDataPixels (t_pixel ** pixels, int height);
t_bmp24 * bmp24_allocate (int width, int height, int colorDepth);
void bmp24_free (t_bmp24 * img);

#endif
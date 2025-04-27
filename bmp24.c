#include "bmp24.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Offsets for the BMP header
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34
// Magical number for BMP files
#define BMP_TYPE 0x4D42 // 'BM' in hexadecimal
// Header sizes
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets
// Constant for the color depth
#define DEFAULT_DEPTH 0x18 // 24


static t_pixel **bmp24_allocateDataPixels(int width, int height) {
    //We allocate memory for each row (height)
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    //Check if allocation failed
    if (!pixels) {
        printf("Error : can't allocate memory");
        return NULL;
    }
    //For each row, allocate the memory for each column (width)
    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        //Check if allocation failed
        if (!pixels[i]) {
            printf("Error : can't allocate memory");
            return NULL;
        }
    }
    //Return the address of the array (first row)
    return pixels;
}

static void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    for (int i = 0; i < height; i++) {
        //Free each row
        free(pixels[i]);
    }
    //Free the array completely
    free(pixels);
}

t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (!img) {
        printf("Error : can't allocate memory");
        return NULL;
    }
    //Save the properties of the image
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(width, height); //Allocate the matrix itself

    if (!img->data) {
        free(img);
        return NULL;
    }
    return img;
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb"); //We need to use "rb" to open a file into "read binary" mode
    if (!file) {
        printf("Error : can't open file : %s", filename);
        return NULL;
    }

    uint16_t type; //Read the type to see if it is a BMP file
    file_rawRead(0, &type, sizeof(uint16_t), 1, file);
    if (type != BMP_TYPE) {
        fclose(file);
        printf("Error : can't read bmp file %s : wrong type", filename);
        return NULL;
    }

    int width, height;
    uint16_t depth;

    //Read the headers to extract the width, height and depth of the image
    file_rawRead(BITMAP_WIDTH, &width, sizeof(int), 1, file);
    file_rawRead(BITMAP_HEIGHT, &height, sizeof(int), 1, file);
    file_rawRead(BITMAP_DEPTH, &depth, sizeof(uint16_t), 1, file);
    if (depth != 24) { //Check if it is, in fact a 24-bit image before any action
        printf("Error : can't read bmp file %s : depth is not 24", filename);
        fclose(file);
        return NULL;
    }
    //Allocate new memory for the image
    t_bmp24 *img = bmp24_allocate(width, height, depth);
    if (!img) {
        printf("Error : can't allocate memory");
        fclose(file);
        return NULL;
    }
    //Read the header and the header info from the file
    file_rawRead(0, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawRead(sizeof(t_bmp_header), &img->header_info, sizeof(t_bmp_info), 1, file);

    fseek(file, img->header.offset, SEEK_SET); //Put the image pointer to the beginning of the file

    //Go through the image and read the color of every pixel (We have to go through it from the bottom to the top i.e y--)
    for (int y = img->height - 1; y >= 0; y--) {
        for (int x = 0; x < img->width; x++) {
            fread(&img->data[y][x].blue, 1, 1, file);
            fread(&img->data[y][x].green, 1, 1, file);
            fread(&img->data[y][x].red, 1, 1, file);
        }
    }

    fclose(file);
    return img;
}
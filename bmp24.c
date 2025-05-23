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


//Given functions
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * f) {
    fseek(f, position, SEEK_SET);
    fread(buffer, size, n, f);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


//Read & wright functions
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *f) {
    fread(&image->data[y][x].blue, 1, 1, f);
    fread(&image->data[y][x].green, 1, 1, f);
    fread(&image->data[y][x].red, 1, 1, f);
}

void bmp24_readPixelData(t_bmp24 *image, FILE *f) {
    fseek(f, image->header.offset, SEEK_SET); //Go to pixel data start (beginning)

    //Go through the image and read the color of every pixel (We have to go through it from the bottom to the top i.e y--)
    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, f);
        }
    }
}


void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *f) {
    fwrite(&image->data[y][x].blue, 1, 1, f);
    fwrite(&image->data[y][x].green, 1, 1, f);
    fwrite(&image->data[y][x].red, 1, 1, f);
}


void bmp24_writePixelData(t_bmp24 *image, FILE *f) {
    fseek(f, image->header.offset, SEEK_SET);

    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, f);
        }
    }
}



//Exercises functions
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
    t_bmp24 *image = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (!image) {
        printf("Error : can't allocate memory");
        return NULL;
    }
    //Save the properties of the image
    image->width = width;
    image->height = height;
    image->colorDepth = colorDepth;
    image->data = bmp24_allocateDataPixels(width, height); //Allocate the matrix itself

    if (!image->data) {
        free(image);
        return NULL;
    }
    return image;
}


t_bmp24 *bmp24_loadImage(const char *f_name) {
    FILE *f = fopen(f_name, "rb"); //We need to use "rb" to open a file into "read binary" mode
    if (!f) {
        printf("Error : can't open file : %s", f_name);
        return NULL;
    }

    uint16_t type; //Read the type to see if it is a BMP file
    file_rawRead(0, &type, sizeof(uint16_t), 1, f);
    if (type != BMP_TYPE) {
        fclose(f);
        printf("Error : can't read bmp file %s : wrong type", f_name);
        return NULL;
    }

    int width, height;
    uint16_t depth;

    //Read the headers to extract the width, height and depth of the image
    file_rawRead(BITMAP_WIDTH, &width, sizeof(int), 1, f);
    file_rawRead(BITMAP_HEIGHT, &height, sizeof(int), 1, f);
    file_rawRead(BITMAP_DEPTH, &depth, sizeof(uint16_t), 1, f);
    if (depth != 24) { //Check if it is, in fact a 24-bit image before any action
        printf("Error : can't read bmp file %s : depth is not 24", f_name);
        fclose(f);
        return NULL;
    }
    //Allocate new memory for the image
    t_bmp24 *image = bmp24_allocate(width, height, depth);
    if (!image) {
        printf("Error : can't allocate memory");
        fclose(f);
        return NULL;
    }
    //Read the header and the header info from the file
    file_rawRead(0, &image->header, sizeof(t_bmp_header), 1, f);
    file_rawRead(sizeof(t_bmp_header), &image->header_info, sizeof(t_bmp_info), 1, f);

    bmp24_readPixelData(image, f); //Read every pixels of the image
    fclose(f);
    return image;
}


void bmp24_saveImage(t_bmp24* image, const char* f_name) {
    FILE *f = fopen(f_name, "wb"); //Open the file in binary write mode and checks if no errors
    if (!f) {
        fclose(f);
        printf("Error : can't read bmp file %s", f_name);
    }

    //Copy the headers
    fwrite(&image->header, sizeof(t_bmp_header), 1, f);
    fwrite(&image->header_info, sizeof(t_bmp_info), 1, f);

    //Rewrite every pixel of the image in the file
    bmp24_writePixelData(image, f);
    fclose(f);
}


void bmp24_negative (t_bmp24 * image) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            image->data[y][x].red = 255 - image->data[y][x].red;
            image->data[y][x].green = 255 - image->data[y][x].green;
            image->data[y][x].blue = 255 - image->data[y][x].blue;
        }
    }
}

void bmp24_grayscale (t_bmp24 * image) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            uint8_t gray = (image->data[y][x].red + image->data[y][x].green + image->data[y][x].blue) / 3;
            image->data[y][x].red = image->data[y][x].green = image->data[y][x].blue = gray;
        }
    }
}

void bmp24_brightness (t_bmp24 * image, int value) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {

            image->data[y][x].red = image->data[y][x].red + value;
            if (image->data[y][x].red > 255) image->data[y][x].red = 255;

            image->data[y][x].green = image->data[y][x].green + value;
            if (image->data[y][x].green > 255) image->data[y][x].green = 255;

            image->data[y][x].blue = image->data[y][x].blue + value;
            if (image->data[y][x].blue > 255) image->data[y][x].blue = 255;
        }
    }
}

t_pixel bmp24_convolution(t_bmp24 *image, int x, int y, float **kernel, int kernelSize) {

    float red = 0, green = 0, blue = 0;
    //Offset is used to take all the pixels around the target pixel
    int offset = 1;

    //We're taking every pixel around the target pixel
    for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {

            //Storing the coordinates of every pixel
            int x_i = x + j;
            int y_i = y + i;

            //Verification (we're verifying that we don't go too far)
            if (x_i >= 0 && x_i < image->width && y_i >= 0 && y_i < image->height) {
                t_pixel p = image->data[y_i][x_i];

                //k takes the value associated in the kernel matrix
                float k = kernel[i + offset][j + offset];

                //We apply the filter to every pixel
                red += p.red * k;
                green += p.green * k;
                blue += p.blue * k;
            }
        }
    }


    //We make sure that the new color is between bounds
    t_pixel result;
    result.red = fmin(fmax(round(red), 0), 255);
    result.green = fmin(fmax(round(green), 0), 255);
    result.blue = fmin(fmax(round(blue), 0), 255);

    return result;
}

void bmp24_applyFilter(t_bmp24 *image, float **kernel, int kernelSize) {
    //We allocate space for the new data of the pixel
    t_pixel **newData = bmp24_allocateDataPixels(image->width, image->height);
    int offset = 1;

    //We are going through every nearby pixel to apply the filter
    for (int y = offset; y < image->height - offset; y++) {
        for (int x = offset; x < image->width - offset; x++) {

            //We call the function convolution to apply the filter
            newData[y][x] = bmp24_convolution(image, x, y, kernel, kernelSize);
        }
    }

    //We free the ancient data and replace it by the new data (with the filter applied)
    bmp24_freeDataPixels(image->data, image->height);
    image->data = newData;
}


void bmp24_equalize(t_bmp24 *image) {
    int width = image->width;
    int height = image->height;
    int totalPixels = width * height;

    //Initialisation of the histograms for each pixel color
    int histogram_R[256] = {0};
    int histogram_G[256] = {0};
    int histogram_B[256] = {0};

    //For each pixel color, we increase the size of the histogram
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            t_pixel p = image->data[y][x];
            histogram_R[p.red]++;
            histogram_G[p.green]++;
            histogram_B[p.blue]++;
        }
    }

    ///We compute the CDF to proportionate each histogram
    int cdf_R[256], cdf_G[256], cdf_B[256];
    cdf_R[0] = histogram_R[0];
    cdf_G[0] = histogram_G[0];
    cdf_B[0] = histogram_B[0];

    //Calculation of CDF
    for (int i = 1; i < 256; i++) {
        cdf_R[i] = cdf_R[i - 1] + histogram_R[i];
        cdf_G[i] = cdf_G[i - 1] + histogram_G[i];
        cdf_B[i] = cdf_B[i - 1] + histogram_B[i];
    }

    //We initialise the equalized histograms for each pixel color
    uint8_t equalized_R[256];
    uint8_t equalized_G[256];
    uint8_t equalized_B[256];

    int cdfMin_R = cdf_R[0];
    int cdfMin_G = cdf_G[0];
    int cdfMin_B = cdf_B[0];

    //We compute each equalised pixel and ensuring that each pixel is between 0 and 255
    for (int i = 0; i < 256; i++) {
        equalized_R[i] = round(((float)(cdf_R[i] - cdfMin_R) / (totalPixels - cdfMin_R)) * 255);
        equalized_G[i] = round(((float)(cdf_G[i] - cdfMin_G) / (totalPixels - cdfMin_G)) * 255);
        equalized_B[i] = round(((float)(cdf_B[i] - cdfMin_B) / (totalPixels - cdfMin_B)) * 255);
    }

    //Finally, we apply the new pixel values to the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            t_pixel *p = &image->data[y][x];
            p->red   = equalized_R[p->red];
            p->green = equalized_G[p->green];
            p->blue  = equalized_B[p->blue];
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
#include <math.h>



// Functions to read and write images

t_bmp8* bmp8_loadImage(const char* f_name) {
    /*This function allows you to read an 8-bit grayscale image from a BMP file. This function will dynamically allocate
     *memory to store an image of type t_bmp8, initialize the fields of this image, and return a pointer to this image.
     *Input : character corresponding to the relative path
     *If an error occurs while reading the file, or if the image is not 8 bits deep, the function will display an error
     *message and return NULL.*/
    FILE* f = fopen(f_name, "rb");
    if (!f) {
        printf("Error: Could not open file %s\n", f_name);
        return NULL;
    }

    t_bmp8* img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Error: Memory allocation failed\n");
        fclose(f);
        return NULL;
    }

    fread(img->header, sizeof(unsigned char), 54, f);
    fread(img->colorTable, sizeof(unsigned char), 1024, f);

    img->width = *(unsigned int*)(&img->header[18]);
    img->height = *(unsigned int*)(&img->header[22]);
    img->colorDepth = *(unsigned int*)(&img->header[28]);
    img->dataSize = img->width * img->height;

    if (img->colorDepth != 8) {
        printf("Error: Not an 8-bit grayscale image\n");
        free(img);
        fclose(f);
        return NULL;
    }

    img->data = (unsigned char*)malloc(img->dataSize);
    if (!img->data) {
        printf("Error: Memory allocation for data failed\n");
        free(img);
        fclose(f);
        return NULL;
    }

    fread(img->data, sizeof(unsigned char), img->dataSize, f);
    fclose(f);
    printf("Image loaded successfully!\n");
    return img;
}


void bmp8_saveImage(const char* f_name, t_bmp8* img) {
    /*This function allows you to write an 8-bit grayscale image to a BMP file whose name is asked.
    Input : pointer to an image of type t_bmp8
    If an error occurs while writing the file, the function will display an error message.*/
    if(!img)
        printf("Image is empty\n");

    FILE* f = fopen(f_name, "wb");
    if (!f) {
        printf("Error: Could not open file %s for writing\n", f_name);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, f);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, f);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, f);

    fclose(f);
    printf("Image saved successfully!\n");
}


void bmp8_free(t_bmp8* img) {
    /*This function allows you to free the memory allocated to store an image of type t_bmp8.
     Input : pointer to an image of type t_bmp8.*/
    if (img) {
        free(img->data);
        free(img);
    }
}


void bmp8_printInfo(t_bmp8* img) {
    /*This function allows you to display the information of an image of type t_bmp8, the width, the height then the
     *color depth and finally the data size.
     *Input : pointer to an image of type t_bmp8 as a parameter and displays the information of this image.*/
    printf("Image Info:\n");
    printf("\tWidth: %u\n", img->width);
    printf("\tHeight: %u\n", img->height);
    printf("\tColor Depth: %u\n", img->colorDepth);
    printf("\tData Size: %u bytes\n", img->dataSize);
}




// Image processing functions

void bmp8_negative(t_bmp8* img) {
    /*This function inverts the colors of a grayscale image. It subtracts the value of each pixel in this image from 255.
     *Input : pointer to an image of type t_bmp8 parameter and */
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = 255 - img->data[i];
    }
    printf("Negative filter applied successfully!\n");
}


void bmp8_brightness(t_bmp8* img, int value) {
    /*This function allows you to modify the brightness of a grayscale image. For each pixel of the image, the function
     *adds a decided value to every pixel. Note that the value of a pixel cannot exceed 255 or be less than 0.
     *Input : pointer to an image of type t_bmp8 and an integer value (which can be negative) as parameters.*/
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        int new_val = img->data[i] + value;
        if (new_val > 255) new_val = 255;
        if (new_val < 0) new_val = 0;
        img->data[i] = (unsigned char)new_val;
    }
    printf("Brightness adjusted successfully!\n");
}


void bmp8_threshold(t_bmp8* img, int threshold) {
    /*This function transforms a grayscale image into a binary image. For each pixel of the image,
     *if the pixel value is greater than or equal to threshold, the pixel value will be set to 255 (white).
     *Otherwise, the pixel value will be set to 0 (black).
     *Input : pointer to an image of type t_bmp8 and an integer threshold. */
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
    printf("Black and white conversion applied successfully!\n");
}


void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    /*This function transform an image in function of a given matrice.
     *Inputs : Takes a pointer to an image of type t_bmp8, a matrix of floats kernel representing the filter kernel,
     *and an integer kernelSize representing the kernel size.*/

    // Checking every possible errors
    if (!img || !img->data || !kernel || kernelSize <= 0 || kernelSize % 2 == 0) {
        fprintf(stderr, "Erreur : paramètres invalides.\n");
        return;
    }

    int const width = (int) img->width;
    int const height = (int) img->height;
    int const expectedSize = width * height;
    if (img->dataSize != expectedSize) {
        fprintf(stderr, "Erreur : taille de données incohérente (%u au lieu de %u).\n", img->dataSize, expectedSize);
        return;
    }

    int n = kernelSize / 2;

    // temporary allocation
    unsigned char *temp = malloc(img->dataSize);
    if (!temp) {
        fprintf(stderr, "Erreur : impossible d'allouer la mémoire pour l'image temporaire.\n");
        return;
    }

    // copy of the image
    for (unsigned int i = 0; i < img->dataSize; i++) {
        temp[i] = img->data[i];
    }

    // Application of the filter
    for (int y = n; y < height - n; y++) {
        for (int x = n; x < width - n; x++) {
            float sum = (float) 0.0;
            for (int i = -n; i <= n; i++) {
                for (int j = -n; j <= n; j++) {
                    int xi = x + j;
                    int yi = y + i;
                    if (xi >= 0 && xi < width && yi >= 0 && yi < height) {
                        unsigned char const pixel = temp[yi * width + xi];
                        sum += (float) pixel * kernel[i + n][j + n];
                    }
                }
            }
            // limiting pixels value
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            img->data[y * width + x] = (unsigned char)roundf(sum);
        }
    }

    // freeing temporary image
    free(temp);
}


// Histogram

unsigned int *bmp8_computeHistogram(t_bmp8 *img) {
    /*This function computes the histogram of an 8-bit grayscale image.
     *Input : pointer to an image t_bmp8 * img
     *Returns an array of integers of size 256 containing the number of pixels for each gray level.
     * Checking for potential errors.
     */
    if (img == NULL || img->data == NULL) {
        return NULL;
    }


    unsigned int *hist = (unsigned int *)calloc(256, sizeof(unsigned int));
    if (hist == NULL) {
        printf("Failed to compute histogram\n");
        return NULL;
    }


    for (unsigned int i = 0; i < img->dataSize; i++) {
        unsigned char pixel = img->data[i];
        hist[pixel]++;
    }

    return hist;
}


unsigned int *bmp8_computeCDF(unsigned int *hist) {
    /*This function calculates the cumulative histogram by summing the values of the original histogram.
     *Input : array of integers hist representing the histogram of a t_bmp8 image
     *Returns :  array of integers of size 256 containing the normalized cumulative histogram hist_eq.*/
    if (hist == NULL) {
        return NULL;
    }


    unsigned int *cdf = (unsigned int *)malloc(256 * sizeof(unsigned int));
    if (cdf == NULL) {
        printf("Failed to compute CDF\n");
        free(hist);
        return NULL;
    }


    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }
    return cdf;
}


void bmp8_equalize(t_bmp8 * img, unsigned int *hist_eq) {
    /*This function applies histogram equalization to an 8-bit grayscale image.
     *Input : pointer to an image t_bmp8 * img representing the original image and an array of integers hist_eq
     *representing the normalized cumulative histogram.*/
    if (img == NULL || img->data == NULL || hist_eq == NULL) {
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (unsigned char)hist_eq[img->data[i]];
    }
}
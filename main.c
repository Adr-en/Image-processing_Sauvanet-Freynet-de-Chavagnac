#include <stdio.h>
#include "bmp8.h"

float box_blur[3][3] = {1/9,1/9,1/9,1/9,1/9,1/9,1/9};
float gaussian_blur[3][3] = {1/16, 1/8, 1/16, 1/4, 1/16, 1/8, 1/16};
float outline[3][3] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
float emboss[3][3] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
float sharpen[3][3] = {0, -1, 0, -1, 5, -1, 0, -1, 0};

int main() {
    // Load the image
    t_bmp8* img = bmp8_loadImage("lena_gray.bmp");
    if (img == NULL) {
        printf("Failed to load image.\n");
        return 1;
    }
    t_bmp8* img_temp = img;

    // Print image information
    bmp8_printInfo(img);

    // Apply negative filter
    bmp8_negative(img);
    printf("Applied negative filter.\n");

    // Save the modified image
    bmp8_saveImage("lena_gray_negative.bmp", img);
    printf("Image saved as barbara_gray_negative.bmp\n");

    img = img_temp;
    bmp8_brightness(img, 50);
    bmp8_saveImage("lena_gray_brightness.bmp", img);

    img = img_temp;
    //bmp8_threshold(img, 128);
    //bmp8_saveImage("lena_gray_threshold.bmp", img);

    printf("good");
    bmp8_applyFilter(img, box_blur, 9);
    printf("okkkk");
    bmp8_saveImage("lena_gray_filtered.bmp", img);
    printf("mamaaaaaaaaaaaaaaaaaaaaaa");


    printf("Filtrage terminé et image enregistrée sous 'barbara_gray_filtered.bmp'.\n");
    return 0;


    // Free the memory
    bmp8_free(img);

    return 0;
}

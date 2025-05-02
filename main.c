#include <stdio.h>
#include "bmp8.h"



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

    //img = img_temp;
    //bmp8_threshold(img, 128);
    //bmp8_saveImage("lena_gray_threshold.bmp", img);


    // Free the memory
    bmp8_free(img);

    return 0;
}

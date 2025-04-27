#include <stdio.h>
#include "bmp8.h"



int main() {
    // Load the image
    t_bmp8* img = bmp8_loadImage("C:\\Users\\adrie\\CLionProjects\\Image-Processing_\\lena_gray.bmp");
    if (img == NULL) {
        printf("Failed to load image.\n");
        return 1;
    }

    // Print image information
    bmp8_printInfo(img);

    // Apply negative filter
    bmp8_negative(img);
    printf("Applied negative filter.\n");

    // Save the modified image
    bmp8_saveImage("lena_gray_negative.bmp", img);
    printf("Image saved as barbara_gray_negative.bmp\n");

    // Free the memory
    bmp8_free(img);

    return 0;
}

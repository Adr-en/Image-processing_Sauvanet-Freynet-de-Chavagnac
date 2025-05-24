#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "menu.h"

/*
int main() {
    t_bmp8* img = bmp8_loadImage("lena_gray.bmp");

    if (!img) {
        printf("Failed to load image.\n");
        return 1;
    }



    bmp8_printInfo(img);
    printf("\n");
    bmp8_negative(img);
    printf("Applied negative filter.\n");
    bmp8_saveImage("lena_gray_negative.bmp", img);
    printf("Image saved as lena_gray_negative.bmp\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applied brightness filter.\n");
    bmp8_brightness(img, 50);
    bmp8_saveImage("lena_gray_brightness.bmp", img);
    printf("Image saved as lena_gray_brightness.bmp\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applied threshold filter.\n");
    bmp8_threshold(img, 128);
    bmp8_saveImage("lena_gray_threshold.bmp", img);
    printf("Image saved as lena_gray_threshold.bmp\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applied outline filter.\n");
    bmp8_applyFilter(img, outline, 3);
    bmp8_saveImage("lena_gray_filtered.bmp", img);
    printf("Image saved as lena_gray_filtered.bmp\n");








    img = bmp8_loadImage("lena_gray.bmp");
    unsigned int *hist = bmp8_computeHistogram(img);

    if (hist == NULL) {
        printf("Failed to compute histogram\n");
        bmp8_free(img);
        return 1;
    }


    unsigned int *cdf = bmp8_computeCDF(hist);

    //Verification
    if (cdf == NULL) {
        printf("Failed to compute CDF\n");
        free(hist);
        bmp8_free(img);
        return 1;
    }

    // Find minimum non-zero value in CDF
    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }


    int nb_pxl = img->width * img->height;


    unsigned int *hist_eq = (unsigned int *)malloc(256 * sizeof(unsigned int));

    //Verification
    if (hist_eq == NULL) {
        printf("Failed to allocate memory for equalized histogram\n");
        free(hist);
        free(cdf);
        bmp8_free(img);
        return 1;
    }


    // Normalize the CDF to create the equalization mapping
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            hist_eq[i] = (unsigned int)round(((float)(cdf[i] - cdf_min) / (nb_pxl - cdf_min)) * 255);
        } else {
            hist_eq[i] = 0;
        }
    }

    printf("Applied equalisation filter.\n");
    bmp8_equalize(img, hist_eq);
    bmp8_saveImage("lena_gray_equalized.bmp", img);


    free(hist);
    free(cdf);
    free(hist_eq);
    bmp8_free(img);

    return 0;
}
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes





int main() {
    int choice;
    box_blur = malloc(3 * sizeof(float *));
    gaussian_blur = malloc(3 * sizeof(float *));
    outline = malloc(3 * sizeof(float *));
    emboss = malloc(3 * sizeof(float *));
    sharpen = malloc(3 * sizeof(float *));
    zero = malloc(3 * sizeof(float *));

    for (int i = 0; i < 3; i++) {
        box_blur[i] = malloc(3 * sizeof(float));
        gaussian_blur[i] = malloc(3 * sizeof(float));
        outline[i] = malloc(3 * sizeof(float));
        emboss[i] = malloc(3 * sizeof(float));
        sharpen[i] = malloc(3 * sizeof(float));
        zero[i] = malloc(3 * sizeof(float));
    }

    float b = 1.0f / 9.0f;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            box_blur[i][j] = b;

    float gb[3][3] = {
        {1.0f/16, 1.0f/8, 1.0f/16},
        {1.0f/8,  1.0f/4, 1.0f/8},
        {1.0f/16, 1.0f/8, 1.0f/16}
    };
    float o[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    float e[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };
    float s[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            gaussian_blur[i][j] = gb[i][j];
            outline[i][j] = o[i][j];
            emboss[i][j] = e[i][j];
            sharpen[i][j] = s[i][j];
            zero[i][j] = 0.0f;
        }



    do {
        displayMainMenu();
        printf(">>> Your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                openImage();
                break;
            case 2:
                saveImage();
                break;
            case 3:
                applyFilter();
                break;
            case 4:
                displayImageInfo();
                break;
            case 5:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 5);

    for (int i = 0; i < 3; i++) {
        free(box_blur[i]);
        free(gaussian_blur[i]);
        free(outline[i]);
        free(emboss[i]);
        free(sharpen[i]);
        free(zero[i]);
    }

    free(box_blur);
    free(gaussian_blur);
    free(outline);
    free(emboss);
    free(sharpen);
    free(zero);

    printf("Everything wass freed");



    return 0;
}

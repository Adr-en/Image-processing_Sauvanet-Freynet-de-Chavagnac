#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

int main() {
    t_bmp8* img = bmp8_loadImage("lena_gray.bmp");
    t_bmp8* img_temp = img;
    if (!img) {
        printf("Failed to load image.\n");
        return 1;
    }

    float **box_blur = malloc(3 * sizeof(float *));
    float **gaussian_blur = malloc(3 * sizeof(float *));
    float **outline = malloc(3 * sizeof(float *));
    float **emboss = malloc(3 * sizeof(float *));
    float **sharpen = malloc(3 * sizeof(float *));
    float **zero = malloc(3 * sizeof(float *));

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

    bmp8_printInfo(img);
    bmp8_negative(img);
    printf("Applied negative filter.\n");
    bmp8_saveImage("lena_gray_negative.bmp", img);
    printf("Image saved as barbara_gray_negative.bmp\n");
    img = img_temp;

    bmp8_brightness(img, 50);
    bmp8_saveImage("lena_gray_brightness.bmp", img);
    img = img_temp;

    bmp8_threshold(img, 128);
    bmp8_saveImage("lena_gray_threshold.bmp", img);

    img = img_temp;
    printf("good");
    bmp8_applyFilter(img, outline, 3);
    printf("okkkk");
    bmp8_saveImage("lena_gray_filtered.bmp", img);
    printf("mamaaaaaaaaaaaaaaaaaaaaaa");

    printf("Filtrage termine et image enregistree sous 'barbara_gray_filtered.bmp'.\n");

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

    bmp8_free(img);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "menu.h"


int main() {


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


    bmp8_test();


    int answer = 0;
    int choice;
    int bit;
    printf("Do you want to use colored images (1) or gray images (0) ?");
    scanf("%d", &answer);

    //Default case will be changed if necessary
    void * img = NULL;
    bit = 8;


    if (answer == 1) {
        img = bmp24_loadImage("C:\\Users\\adrie\\CLionProjects\\Image-Processing_\\lena_color.bmp");
        bit = 24;

    }
    else {
        img = bmp8_loadImage("lena_gray.bmp");
        bmp8_printInfo(img);

    }



    do {
        displayMainMenu();
        printf(">>> Your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                if (bit == 8)
                    img = bmp8_openImage();
                else
                    img = bmp24_openImage();
                break;
            case 2:
                if (!img)
                    printf("Image is empty\n");
                saveImage(bit,img);
                break;
            case 3:
                applyFilter(bit, img);
                break;
            case 4:
                displayImageInfo(bit, img);
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
    free(img);

    printf("Everything wass freed");



    return 0;
}

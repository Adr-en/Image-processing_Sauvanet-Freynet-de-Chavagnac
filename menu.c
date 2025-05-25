//
// Created by adrie on 24/05/2025.
//
#include <stdio.h>

#include "menu.h"


float **box_blur ;
float **gaussian_blur ;
float **outline ;
float **emboss ;
float **sharpen ;
float **zero ;

void displayMainMenu() {
    printf("\nPlease choose an option:\n");
    printf("1. Open an image\n");
    printf("2. Save an image\n");
    printf("3. Apply a filter\n");
    printf("4. Display image information\n");
    printf("5. Quit\n");
}

void displayFilterMenu() {
    printf("\nPlease choose a filter:\n");
    printf("1. Negative\n");
    printf("2. Brightness\n");
    printf("3. Black and white\n");
    printf("4. Box Blur\n");
    printf("5. Gaussian blur\n");
    printf("6. Sharpness\n");
    printf("7. Outline\n");
    printf("8. Emboss\n");
    printf("9. Return to the previous menu\n");
}

t_bmp8 *bmp8_openImage() {
    char filepath[256];
    printf("File path: ");
    scanf("%255s", filepath);
    return bmp8_loadImage(filepath);
}


t_bmp24 *bmp24_openImage() {
    char filepath[256];
    printf("File path: ");
    scanf("%255s", filepath);
    return bmp24_loadImage(filepath);
}



void saveImage(int const bit, void * img) {
    char filename[256];
    printf("On what name do you want to register it ? (put .bmp at the end of the name)\n");
    scanf("%s", filename);


    if (bit == 8) {
      bmp8_saveImage(filename, (t_bmp8 *) img);
    }
    if (bit == 24) {
        bmp24_saveImage((t_bmp24 *) img, filename);
    }

}

void bmp8_handleFilterChoice(int const choice, t_bmp8 *img) {
    int number;
    switch(choice) {
        case 1:
            bmp8_negative(img);
        break;
        case 2:
            printf("How much do you want to change the brightness (between 0 and 255) ?\n");
            scanf("%d", &number);
            bmp8_brightness(img, number);
        break;
        case 3:
            printf("Which threshold do you want to chose (between 0 and 255) ?\n");
            scanf("%d", &number);
            bmp8_threshold(img, number);
        break;
        case 4:
            bmp8_applyFilter(img, box_blur, 3);
            printf("Box blur filter applied successfully!\n");
        break;
        case 5:
            bmp8_applyFilter(img, gaussian_blur, 3);
            printf("Gaussian blur filter applied successfully!\n");
        break;
        case 6:
            bmp8_applyFilter(img, sharpen, 3);
            printf("Sharpness filter applied successfully!\n");
        break;
        case 7:
            bmp8_applyFilter(img, outline, 3);
            printf("Outline filter applied successfully!\n");
        break;
        case 8:
            bmp8_applyFilter(img, emboss, 3);
            printf("Emboss filter applied successfully!\n");
        break;

        default:
            printf("Wrong choice\n");
        break;
    }
}



void bmp24_handleFilterChoice(int const choice, t_bmp24 *img) {
    int number;
    switch(choice) {
        case 1:
            bmp24_negative(img);
        break;
        case 2:
            printf("How much do you want to change the brightness (between 0 and 255) ?\n");
            scanf("%d", &number);
            bmp24_brightness(img, number);
        break;
        case 3:
            bmp24_grayscale(img);
        break;
        case 4:
            bmp24_applyFilter(img, box_blur, 3);
        printf("Box blur filter applied successfully!\n");
        break;
        case 5:
            bmp24_applyFilter(img, gaussian_blur, 3);
        printf("Gaussian blur filter applied successfully!\n");
        break;
        case 6:
            bmp24_applyFilter(img, sharpen, 3);
        printf("Sharpness filter applied successfully!\n");
        break;
        case 7:
            bmp24_applyFilter(img, outline, 3);
        printf("Outline filter applied successfully!\n");
        break;
        case 8:
            bmp24_applyFilter(img, emboss, 3);
        printf("Emboss filter applied successfully!\n");
        break;
        default:
            printf("Wrong choice\n");
        break;
    }
}

void applyFilter(int bit, void *img) {
    int choice_filter;
    //tempo
    bmp8_printInfo(img);

    do {
        displayFilterMenu();
        printf(">>> Your choice: ");
        scanf("%d", &choice_filter);

        if(choice_filter >= 1 && choice_filter <= 8) {
            if (bit == 8)
                bmp8_handleFilterChoice(choice_filter, img);
            else
                bmp24_handleFilterChoice(choice_filter, img);
             }
        else if(choice_filter != 9) {
            printf("Invalid choice. Please try again.\n");
        }
    } while(choice_filter != 9);
}


void displayImageInfo(int bit, void * img) {
    if (bit == 8)
        bmp8_printInfo(img);
    else
        printf("Nope, nothing to say");
    printf("Image information displayed successfully!\n");
}







void test_gray() {

    t_bmp8* img = bmp8_loadImage("lena_gray.bmp");

    if (!img) {
        printf("Failed to load image.\n");
        return;
    }

    bmp8_printInfo(img);
    printf("\n\n");

    bmp8_negative(img);
    printf("Applying negative filter.\n");
    bmp8_saveImage("lena_gray_negative.bmp", img);
    printf("Image saved as lena_gray_negative.bmp\n\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applying brightness filter.\n");
    bmp8_brightness(img, 50);
    bmp8_saveImage("lena_gray_brightness.bmp", img);
    printf("Image saved as lena_gray_brightness.bmp\n\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applying threshold filter.\n");
    bmp8_threshold(img, 128);
    bmp8_saveImage("lena_gray_threshold.bmp", img);
    printf("Image saved as lena_gray_threshold.bmp\n\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applying outline filter.\n");
    bmp8_applyFilter(img, outline, 3);
    bmp8_saveImage("lena_gray_filtered.bmp", img);
    printf("Image saved as lena_gray_outline.bmp\n\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applying box blur filter.\n");
    bmp8_applyFilter(img, box_blur, 3);
    bmp8_saveImage("lena_gray_blur.bmp", img);
    printf("Image saved as lena_gray_blur.bmp\n\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applying Gaussian blur filter.\n");
    bmp8_applyFilter(img, gaussian_blur, 3);
    bmp8_saveImage("lena_gray_gaussian.bmp", img);
    printf("Image saved as lena_gray_gaussian.bmp\n\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applying sharpen filter.\n");
    bmp8_applyFilter(img, sharpen, 3);
    bmp8_saveImage("lena_gray_sharpen.bmp", img);
    printf("Image saved as lena_gray_sharpen.bmp\n\n");


    img = bmp8_loadImage("lena_gray.bmp");
    printf("Applying emboss filter.\n");
    bmp8_applyFilter(img, emboss, 3);
    bmp8_saveImage("lena_gray_emboss.bmp", img);
    printf("Image saved as lena_gray_emboss.bmp\n\n");



    img = bmp8_loadImage("lena_gray.bmp");
    unsigned int *hist = bmp8_computeHistogram(img);
    unsigned int *cdf = bmp8_computeCDF(hist);

        // Find minimum non-zero value in CDF
    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }


    unsigned int *hist_eq = (unsigned int *)malloc(256 * sizeof(unsigned int));

    //Verification
    if (hist_eq == NULL) {
        printf("Failed to allocate memory for equalized histogram\n");
        free(hist);
        free(cdf);
        bmp8_free(img);
        return;
    }


    // Normalize the CDF to create the equalization mapping
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            hist_eq[i] = (unsigned int)roundf(((float)(cdf[i] - cdf_min) / ((float)img -> dataSize - (float)cdf_min)) * 255);
        } else {
            hist_eq[i] = 0;
        }
    }

    printf("Applying equalisation filter.\n");
    bmp8_equalize(img, hist_eq);
    bmp8_saveImage("lena_gray_equalized.bmp", img);
    printf("Image saved as lena_gray_equalized.bmp\n\n");


    free(hist);
    free(cdf);
    free(hist_eq);
    bmp8_free(img);

    printf("Everything was freed");
    printf("Everything work well, you can go see images in the cmake-build-debug repository");
}
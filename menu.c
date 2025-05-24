//
// Created by adrie on 24/05/2025.
//
#include <stdio.h>
#include "bmp8.h"
#include "bmp24.h"
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

void openImage() {
    char filepath[256];
    int bit;
    printf("File path: ");
    scanf("%255s", filepath);
    printf("How much bit is it on?\n");
    scanf("%d", &bit);
    if (bit == 8) {
        bmp8_loadImage(filepath);
    }
    if (bit == 24) {
        bmp24_loadImage(filepath);
    }

}

void saveImage() {
    char filepath[256];
    char filename[256];
    int bit;

    printf("File path: ");
    scanf("%255s", filepath);

    printf("On what name do you want to register it ?\n");
    scanf("%s", filename);

    printf("How much bit is it on?\n");
    scanf("%d", &bit);

    if (bit == 8) {
      bmp8_saveImage(filename, bmp8_loadImage(filepath));
    }
    if (bit == 24) {
        bmp24_saveImage(bmp24_loadImage(filename),filename);
    }

}

void bmp8_handleFilterChoice(int choice, char* filepath) {
    int number;
    switch(choice) {
        case 1:
            bmp8_negative(bmp8_loadImage(filepath));
        break;
        case 2:
            printf("How much do you want to change the brightness (between 0 and 255) ?\n");
            scanf("%d", &number);
            bmp8_brightness(bmp8_loadImage(filepath), number);
        break;
        case 3:
            printf("Which threshold do you want to chose (between 0 and 255) ?\n");
            scanf("%d", &number);
            bmp8_threshold(bmp8_loadImage(filepath), number);
        break;
        case 4:
            bmp8_applyFilter(bmp8_loadImage(filepath), box_blur, 3);
            printf("Box blur filter applied successfully!\n");
        break;
        case 5:
            bmp8_applyFilter(bmp8_loadImage(filepath), gaussian_blur, 3);
            printf("Gaussian blur filter applied successfully!\n");
        break;
        case 6:
            bmp8_applyFilter(bmp8_loadImage(filepath), sharpen, 3);
            printf("Sharpness filter applied successfully!\n");
        break;
        case 7:
            bmp8_applyFilter(bmp8_loadImage(filepath), outline, 3);
            printf("Outline filter applied successfully!\n");
        break;
        case 8:
            bmp8_applyFilter(bmp8_loadImage(filepath), emboss, 3);
            printf("Emboss filter applied successfully!\n");
        break;
    }
}



void bmp24_handleFilterChoice(int choice, char* filepath) {
    int number;
    switch(choice) {
        case 1:
            bmp24_negative(bmp24_loadImage(filepath));
        break;
        case 2:
            printf("How much do you want to change the brightness (between 0 and 255) ?\n");
            scanf("%d", &number);
            bmp24_brightness(bmp24_loadImage(filepath), number);
        break;
        case 3:
            bmp24_grayscale(bmp24_loadImage(filepath));
        break;
        case 4:
            bmp24_convolution(bmp24_loadImage(filepath),0,0, box_blur, 3);
        printf("Box blur filter applied successfully!\n");
        break;
        case 5:
            bmp24_convolution(bmp24_loadImage(filepath),0,0, gaussian_blur, 3);
        printf("Gaussian blur filter applied successfully!\n");
        break;
        case 6:
            bmp24_convolution(bmp24_loadImage(filepath),0,0, sharpen, 3);
        printf("Sharpness filter applied successfully!\n");
        break;
        case 7:
            bmp24_convolution(bmp24_loadImage(filepath),0,0, outline, 3);
        printf("Outline filter applied successfully!\n");
        break;
        case 8:
            bmp24_convolution(bmp24_loadImage(filepath),0,0, emboss, 3);
        printf("Emboss filter applied successfully!\n");
        break;
    }
}

void applyFilter() {
    char filepath[256];
    int choice_filter;
    int bit;

    printf("Which image do you want to modify?\n");
    scanf("%s", filepath);

    do {
        displayFilterMenu();
        printf(">>> Your choice: ");
        scanf("%d", &choice_filter);

        if(choice_filter >= 1 && choice_filter <= 8) {
            printf("How much bit takes the image (8 or 24)?\n");
            scanf("%d", &bit);
            if (bit == 8) {
                bmp8_handleFilterChoice(choice_filter, filepath);
             }
            else {
                if (bit == 24) {
                    bmp24_handleFilterChoice(choice_filter, filepath);
                }
                else {printf("Can't manage this kind of images");
                }
            }

        } else if(choice_filter != 9) {
            printf("Invalid choice. Please try again.\n");
        }
    } while(choice_filter != 9);
}


void displayImageInfo() {
    // Here you would display image information
    printf("Image information displayed successfully!\n");
}
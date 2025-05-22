# include <stdio.h>
# include <stdlib.h>
#include "bmp8.h"



t_bmp8* bmp8_loadImage(const char* f_name) {
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
    img->dataSize = *(unsigned int*)(&img->header[34]);

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
    return img;
}




void bmp8_saveImage(const char* f_name, t_bmp8* img) {
    FILE* f = fopen(f_name, "wb");
    if (!f) {
        printf("Error: Could not open file %s for writing\n", f_name);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, f);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, f);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, f);

    fclose(f);
}



void bmp8_free(t_bmp8* img) {
    if (img) {
        free(img->data);
        free(img);
    }
}



void bmp8_printInfo(t_bmp8* img) {
    printf("Image Info:\n");
    printf("\tWidth: %u\n", img->width);
    printf("\tHeight: %u\n", img->height);
    printf("\tColor Depth: %u\n", img->colorDepth);
    printf("\tData Size: %u bytes\n", img->dataSize);
}



void bmp8_negative(t_bmp8* img) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = 255 - img->data[i];
    }
}




void bmp8_brightness(t_bmp8* img, int value) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        int new_val = img->data[i] + value;
        if (new_val > 255) new_val = 255;
        if (new_val < 0) new_val = 0;
        img->data[i] = (unsigned char)new_val;
    }
}


void bmp8_threshold(t_bmp8* img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}


#include <math.h>


void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    // Vérification des paramètres
    if (!img || !img->data || !kernel || kernelSize <= 0 || kernelSize % 2 == 0) {
        fprintf(stderr, "Erreur : paramètres invalides.\n");
        return;
    }

    int width = img->width;
    int height = img->height;
    int expectedSize = width * height;
    if (img->dataSize != expectedSize) {
        fprintf(stderr, "Erreur : taille de données incohérente (%u au lieu de %u).\n", img->dataSize, expectedSize);
        return;
    }

    int n = kernelSize / 2;

    // Allocation de mémoire pour une copie temporaire de l'image
    unsigned char *temp = malloc(img->dataSize);
    if (!temp) {
        fprintf(stderr, "Erreur : impossible d'allouer la mémoire pour l'image temporaire.\n");
        return;
    }

    // Copie initiale de l'image
    for (unsigned int i = 0; i < img->dataSize; i++) {
        temp[i] = img->data[i];
    }

    // Application du filtre par convolution (sans gérer les bords)
    for (int y = n; y < height - n; y++) {
        for (int x = n; x < width - n; x++) {
            float sum = 0.0;
            for (int i = -n; i <= n; i++) {
                for (int j = -n; j <= n; j++) {
                    int xi = x + j;
                    int yi = y + i;
                    if (xi >= 0 && xi < width && yi >= 0 && yi < height) {
                        unsigned char pixel = temp[yi * width + xi];
                        sum += pixel * kernel[i + n][j + n];
                    }
                }
            }
            // Clamping entre 0 et 255
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            img->data[y * width + x] = (unsigned char)roundf(sum);
        }
    }

    // Libération de l'image temporaire
    free(temp);
}



//
// Created by Raphaël on 02/05/2025.
//

unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    unsigned int *histogram = calloc(256, sizeof(unsigned int));
    for (int i = 0; i < img->width; i++) {
        for (int j = 0; j < img->height; j++) {
            unsigned char val = img->data[j * img->width + i];
            histogram[val]++;
        }

    }
    return histogram;
}


unsigned int * bmp8_computeCDF(unsigned int * hist) {
    //compute cdf
    unsigned int* cdf = calloc(256, sizeof(int));
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = hist[i]+cdf[i-1];
    }
    //compute cdf_min
    for ( int i = 0; i < 256; i++) {
        //if cdf
    }

    //compute normalize histogramme
    unsigned int *hist_norm =  calloc(256, sizeof(int)); ;
    for (int i = 1; i < 256; i++) {
       // hist_norm[i] =
    }


    free(cdf);
    free(hist_norm);
    return hist_norm;


}


void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);


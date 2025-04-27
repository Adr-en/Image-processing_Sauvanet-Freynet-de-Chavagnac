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


void bmp8_applyFilter(t_bmp8* img, float** kernel, int kernelSize) {
    int n = kernelSize / 2;
    unsigned char* newData = (unsigned char*)malloc(img->dataSize);
    if (!newData) {
        printf("Error: Memory allocation failed for filtering\n");
        return;
    }

    // Copy original data for border pixels
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        newData[i] = img->data[i];
    }

    for (unsigned int y = 1; y < img->height - 1; ++y) {
        for (unsigned int x = 1; x < img->width - 1; ++x) {
            float sum = 0.0f;
            for (int j = -n; j <= n; ++j) {
                for (int i = -n; i <= n; ++i) {
                    unsigned int pixel = img->data[(y - j) * img->width + (x - i)];
                    sum += kernel[j + n][i + n] * pixel;
                }
            }
            // Clamp to [0,255]
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            newData[y * img->width + x] = (unsigned char)sum;
        }
    }

    free(img->data);
    img->data = newData;
}

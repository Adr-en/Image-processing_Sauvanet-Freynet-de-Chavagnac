# include <stdio.h>
# include <stdlib.h>
#include "bmp8.h"


/*t_bmp8 * bmp8_loadImage(const char * filename){
  t_bmp8 * p_image = (t_bmp8 *) malloc(sizeof(t_bmp8));
  if (p_image == NULL){
    printf("Memory allocation error\n");
    return NULL;
  }

  return p_image;
  }



 //La prof a écrit ça au tableau
 ////FILE *f = fopen("lena_color.bmp", "rb");
 // unsigned header[54];
 //     fread(header, 1, 54, f); //54 element of 1 byte

  //int width = *(unsigned int *) (&header[18]);     //(unsigned int *) correspond to 4 bytes
  //int height = *(unsigned int *) (&header[22]);    // when we fread a file the index pointed by &header is the last one (54) we have to open it again to go back to index 0


*/




t_bmp8* bmp8_loadImage(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    t_bmp8* img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(img->header, sizeof(unsigned char), 54, file);
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    img->width = *(unsigned int*)&img->header[18];
    img->height = *(unsigned int*)&img->header[22];
    img->colorDepth = *(unsigned int*)&img->header[28];
    img->dataSize = *(unsigned int*)&img->header[34];

    if (img->colorDepth != 8) {
        printf("Error: Not an 8-bit grayscale image\n");
        free(img);
        fclose(file);
        return NULL;
    }

    img->data = (unsigned char*)malloc(img->dataSize);
    if (!img->data) {
        printf("Error: Memory allocation for data failed\n");
        free(img);
        fclose(file);
        return NULL;
    }

    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    return img;
}







//2

void bmp8_saveImage(const char* filename, t_bmp8* img) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
}





//3

void bmp8_free(t_bmp8* img) {
    if (img) {
        free(img->data);
        free(img);
    }
}







//4

void bmp8_printInfo(t_bmp8* img) {
    printf("Image Info:\n");
    printf("Width: %u\n", img->width);
    printf("Height: %u\n", img->height);
    printf("Color Depth: %u\n", img->colorDepth);
    printf("Data Size: %u bytes\n", img->dataSize);
}




//5

void bmp8_negative(t_bmp8* img) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = 255 - img->data[i];
    }
}






//6

void bmp8_brightness(t_bmp8* img, int value) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        int newValue = img->data[i] + value;
        if (newValue > 255) newValue = 255;
        if (newValue < 0) newValue = 0;
        img->data[i] = (unsigned char)newValue;
    }
}





//7

void bmp8_threshold(t_bmp8* img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}


//8


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

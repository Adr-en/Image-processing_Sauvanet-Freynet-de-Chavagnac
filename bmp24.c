#include "bmp24.h"


// Offsets for the BMP header
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34
// Magical number for BMP files
#define BMP_TYPE 0x4D42 // 'BM' in hexadecimal
// Header sizes
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets
// Constant for the color depth
#define DEFAULT_DEPTH 0x18 // 24


//Given functions
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * f) {
    fseek(f, position, SEEK_SET);
    fread(buffer, size, n, f);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


//Read & wright functions
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *f) {
    fread(&image->data[y][x].blue, 1, 1, f);
    fread(&image->data[y][x].green, 1, 1, f);
    fread(&image->data[y][x].red, 1, 1, f);
}

void bmp24_readPixelData(t_bmp24 *image, FILE *f) {
    fseek(f, image->header.offset, SEEK_SET); //Go to pixel data start (beginning)

    //Go through the image and read the color of every pixel (We have to go through it from the bottom to the top i.e y--)
    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, f);
        }
    }
}


void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *f) {
    fwrite(&image->data[y][x].blue, 1, 1, f);
    fwrite(&image->data[y][x].green, 1, 1, f);
    fwrite(&image->data[y][x].red, 1, 1, f);
}


void bmp24_writePixelData(t_bmp24 *image, FILE *f) {
    fseek(f, image->header.offset, SEEK_SET);

    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, f);
        }
    }
}



//Exercises functions
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    //We allocate memory for each row (height)
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    //Check if allocation failed
    if (!pixels) {
        printf("Error : can't allocate memory");
        return NULL;
    }
    //For each row, allocate the memory for each column (width)
    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        //Check if allocation failed
        if (!pixels[i]) {
            printf("Error : can't allocate memory");
            return NULL;
        }
    }
    //Return the address of the array (first row)
    return pixels;
}


void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    for (int i = 0; i < height; i++) {
        //Free each row
        free(pixels[i]);
    }
    //Free the array completely
    free(pixels);
}


t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *image = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (!image) {
        printf("Error : can't allocate memory");
        return NULL;
    }
    //Save the properties of the image
    image->width = width;
    image->height = height;
    image->colorDepth = colorDepth;
    image->data = bmp24_allocateDataPixels(width, height); //Allocate the matrix itself

    if (!image->data) {
        free(image);
        return NULL;
    }
    return image;
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("Erreur ouverture fichier %s\n", filename);
        return NULL;
    }

    // Manually read only the essential header fields
    uint16_t type;
    int32_t width, height;
    uint16_t bits;
    uint32_t compression, offset;

    fseek(f, 0, SEEK_SET);
    // BMP signature
    fread(&type, sizeof(uint16_t), 1, f);

    fseek(f, 18, SEEK_SET);
    fread(&width, sizeof(int32_t), 1, f);
    fread(&height, sizeof(int32_t), 1, f);

    fseek(f, 28, SEEK_SET);
    fread(&bits, sizeof(uint16_t), 1, f);

    fseek(f, 30, SEEK_SET);
    fread(&compression, sizeof(uint32_t), 1, f);

    fseek(f, 10, SEEK_SET);
    fread(&offset, sizeof(uint32_t), 1, f);

    // Validate BMP 24-bit uncompressed format
    if (type != 0x4D42 || bits != 24 || compression != 0) {
        printf("Incompatible file. BMP 24 bits must be uncompressed .\n");
        fclose(f);
        return NULL;
    }

    // Allocate the structure
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    img->width = width;
    img->height = height;
    img->colorDepth = bits;
    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        fclose(f);
        free(img);
        return NULL;
    }

    // // Read pixel data starting at "offset", line by line (a big issues in the process of the bm24.c
    fseek(f, offset, SEEK_SET);
    int padding = (4 - (width * 3) % 4) % 4;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char bgr[3];
            fread(bgr, 1, 3, f);
            img->data[height - 1 - y][x].blue = bgr[0];
            img->data[height - 1 - y][x].green = bgr[1];
            img->data[height - 1 - y][x].red = bgr[2];
        }
        // skip padding bytes
        fseek(f, padding, SEEK_CUR);
    }

    fclose(f);
    printf("Image loaded : %dx%d\n", width, height);
    return img;
}

void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Writing error  %s\n", filename);
        return;
    }

    // // Write BMP header (14 bytes)
    uint16_t type = 0x4D42;
    uint32_t offset = 54;
    uint32_t size = offset + (img->width * 3 + (4 - (img->width * 3 % 4)) % 4) * img->height;
    uint16_t reserved = 0;

    // Write BMP info header (40 bytes)
    fwrite(&type, sizeof(uint16_t), 1, f);
    fwrite(&size, sizeof(uint32_t), 1, f);
    fwrite(&reserved, sizeof(uint16_t), 1, f);
    fwrite(&reserved, sizeof(uint16_t), 1, f);
    fwrite(&offset, sizeof(uint32_t), 1, f);

    // Write header info (40 octets)
    uint32_t headerSize = 40;
    uint16_t planes = 1;
    uint16_t bits = 24;
    uint32_t compression = 0;
    uint32_t imageSize = size - offset;
    int32_t resolution = 2835;

    fwrite(&headerSize, sizeof(uint32_t), 1, f);
    fwrite(&img->width, sizeof(int32_t), 1, f);
    fwrite(&img->height, sizeof(int32_t), 1, f);
    fwrite(&planes, sizeof(uint16_t), 1, f);
    fwrite(&bits, sizeof(uint16_t), 1, f);
    fwrite(&compression, sizeof(uint32_t), 1, f);
    fwrite(&imageSize, sizeof(uint32_t), 1, f);
    fwrite(&resolution, sizeof(int32_t), 1, f);
    fwrite(&resolution, sizeof(int32_t), 1, f);
    // ncolors = 0
    fwrite(&compression, sizeof(uint32_t), 1, f);
    // important colors = 0
    fwrite(&compression, sizeof(uint32_t), 1, f);

    // Write pixels
    int padding = (4 - (img->width * 3) % 4) % 4;
    unsigned char pad[3] = {0, 0, 0};

    for (int y = img->height - 1; y >= 0; y--) {
        for (int x = 0; x < img->width; x++) {
            unsigned char bgr[3] = {
                    img->data[y][x].blue,
                    img->data[y][x].green,
                    img->data[y][x].red
            };
            fwrite(bgr, 1, 3, f);
        }
        fwrite(pad, 1, padding, f);
    }

    fclose(f);
    printf("Image save successfully in %s\n", filename);
}
void bmp24_printInfo(t_bmp24* image) {
    if (!image) {
        printf("Image is empty\n");
        return;
    }
    printf("Image width : %d\n", image->width);
    printf("Image height : %d\n", image->height);
    printf("Image depth : %d\n", image->colorDepth);

}


void bmp24_negative (t_bmp24 * image) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            image->data[y][x].red = 255 - image->data[y][x].red;
            image->data[y][x].green = 255 - image->data[y][x].green;
            image->data[y][x].blue = 255 - image->data[y][x].blue;
        }
    }
}

void bmp24_grayscale (t_bmp24 * image) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            uint8_t gray = (image->data[y][x].red + image->data[y][x].green + image->data[y][x].blue) / 3;
            image->data[y][x].red = image->data[y][x].green = image->data[y][x].blue = gray;
        }
    }
}

void bmp24_brightness (t_bmp24 * image, int value) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {

            image->data[y][x].red = image->data[y][x].red + value;
            if (image->data[y][x].red > 255) image->data[y][x].red = 255;

            image->data[y][x].green = image->data[y][x].green + value;
            if (image->data[y][x].green > 255) image->data[y][x].green = 255;

            image->data[y][x].blue = image->data[y][x].blue + value;
            if (image->data[y][x].blue > 255) image->data[y][x].blue = 255;
        }
    }
}

t_pixel bmp24_convolution(t_bmp24 *image, int x, int y, float **kernel, int kernelSize) {

    float red = 0, green = 0, blue = 0;
    //Offset is used to take all the pixels around the target pixel
    int offset = 1;

    //We're taking every pixel around the target pixel
    for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {

            //Storing the coordinates of every pixel
            int x_i = x + j;
            int y_i = y + i;

            //Verification (we're verifying that we don't go too far)
            if (x_i >= 0 && x_i < image->width && y_i >= 0 && y_i < image->height) {
                t_pixel p = image->data[y_i][x_i];

                //k takes the value associated in the kernel matrix
                float k = kernel[i + offset][j + offset];

                //We apply the filter to every pixel
                red += p.red * k;
                green += p.green * k;
                blue += p.blue * k;
            }
        }
    }


    //We make sure that the new color is between bounds
    t_pixel result;
    result.red = fmin(fmax(round(red), 0), 255);
    result.green = fmin(fmax(round(green), 0), 255);
    result.blue = fmin(fmax(round(blue), 0), 255);

    return result;
}

void bmp24_applyFilter(t_bmp24 *image, float **kernel, int kernelSize) {
    //We allocate space for the new data of the pixel
    t_pixel **newData = bmp24_allocateDataPixels(image->width, image->height);
    int offset = 1;

    //We are going through every nearby pixel to apply the filter
    for (int y = offset; y < image->height - offset; y++) {
        for (int x = offset; x < image->width - offset; x++) {

            //We call the function convolution to apply the filter
            newData[y][x] = bmp24_convolution(image, x, y, kernel, kernelSize);
        }
    }

    //We free the ancient data and replace it by the new data (with the filter applied)
    bmp24_freeDataPixels(image->data, image->height);
    image->data = newData;
}


void bmp24_equalize(t_bmp24 *image) {
    int width = image->width;
    int height = image->height;
    int totalPixels = width * height;

    //Initialisation of the histograms for each pixel color
    int histogram_R[256] = {0};
    int histogram_G[256] = {0};
    int histogram_B[256] = {0};

    //For each pixel color, we increase the size of the histogram
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            t_pixel p = image->data[y][x];
            histogram_R[p.red]++;
            histogram_G[p.green]++;
            histogram_B[p.blue]++;
        }
    }

    ///We compute the CDF to proportionate each histogram
    int cdf_R[256], cdf_G[256], cdf_B[256];
    cdf_R[0] = histogram_R[0];
    cdf_G[0] = histogram_G[0];
    cdf_B[0] = histogram_B[0];

    //Calculation of CDF
    for (int i = 1; i < 256; i++) {
        cdf_R[i] = cdf_R[i - 1] + histogram_R[i];
        cdf_G[i] = cdf_G[i - 1] + histogram_G[i];
        cdf_B[i] = cdf_B[i - 1] + histogram_B[i];
    }

    //We initialise the equalized histograms for each pixel color
    uint8_t equalized_R[256];
    uint8_t equalized_G[256];
    uint8_t equalized_B[256];

    int cdfMin_R = cdf_R[0];
    int cdfMin_G = cdf_G[0];
    int cdfMin_B = cdf_B[0];

    //We compute each equalised pixel and ensuring that each pixel is between 0 and 255
    for (int i = 0; i < 256; i++) {
        equalized_R[i] = round(((float)(cdf_R[i] - cdfMin_R) / (totalPixels - cdfMin_R)) * 255);
        equalized_G[i] = round(((float)(cdf_G[i] - cdfMin_G) / (totalPixels - cdfMin_G)) * 255);
        equalized_B[i] = round(((float)(cdf_B[i] - cdfMin_B) / (totalPixels - cdfMin_B)) * 255);
    }

    //Finally, we apply the new pixel values to the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            t_pixel *p = &image->data[y][x];
            p->red   = equalized_R[p->red];
            p->green = equalized_G[p->green];
            p->blue  = equalized_B[p->blue];
        }
    }
}

void bmp24_free(t_bmp24* img) {
    if (img) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}
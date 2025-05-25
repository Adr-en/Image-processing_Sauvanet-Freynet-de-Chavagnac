# BMP Image Processor Technical Documentation

# Objectif :
Make a functional menu allowing to apply different modification, through different filters, over gray and colored images.

## 1. Libraries Overview

The project utilizes several libraries :

- stdio.h : Provides essential file operations for reading and writing BMP files, including fopen, fread, fwrite, and fclose functions.

- stdlib.h: Handles memory management through malloc, calloc, and free functions, crucial for image data manipulation.

- math.h: Enables complex mathematical operations required for filter algorithms and histogram equalization calculations (mainly the function round() for us).


## 2. Core Functionality

### 2.1 8-bit Grayscale Image Processing

The bmp8 module handles grayscale image operations through these functions:

- The bmp8_loadImage function : accepts a filename string as input and returns a pointer to a newly allocated t_bmp8 structure containing the loaded image data. 
For image output,
-  bmp8_saveImage : takes a filename and t_bmp8 pointer, writing the image data to disk in proper BMP format while preserving all header information.

- bmp8_free : takes a bmp8 pointer and free it, return nothing
- bmp8_printInfo : takes a bmp8 pointer and print its width, its height, its color depth and its data size

Image processing operations include:
- bmp8_negative: Inverts pixel values (255 - current value), return nothing
- bmp8_brightness: Adjusts luminosity by a specified integer delta, return nothing
- bmp8_treshold : Make every pixel black below a certain treshold and white if above
- bmp8_applyFilter: Applies convolution matrices for effects like blurring and sharpening, return nothing

### 2.2 24-bit Color Image Processing

The bmp24 module manages operations for 24-bit color BMP images, where each pixel stores separate red, green, and blue values. It extends the grayscale functionality to color images.

The primary functions include:
- bmp24_loadImage / bmp24_saveImage: Load and save color BMP images while preserving header structure and correct pixel formatting. Each pixel is read or written as a combination of three 8-bit channels (BGR order in BMP).
- bmp24_printInfo : Displays image width, height, color depth (should be 24), and image data size.

Image processing operations include:
- bmp24_negative: Inverts the image’s colors by subtracting each RGB value from 255, producing a photographic negative effect.
- bmp24_grayscale: Converts the color image to grayscale using the weighted luminance formula (0.299R + 0.587G + 0.114B), resulting in a single intensity value applied to all three channels.
- bmp24_brightness: Adjusts brightness across all color channels by adding or subtracting a constant value while keeping pixel values within the 0–255 range.
- bmp24_applyFilter: Applies convolution filters (like blur, sharpen, or edge detection) over each RGB channel independently using a user-defined kernel, enhancing or modifying local pixel contrast.

This module enables advanced image enhancement while preserving color integrity.

### 2.3 Histogram Operations

The histogram processing system consists of three core functions:

- bmp8_computeHistogram analyzes pixel value distribution across the 0-255 range, returning a 256-element frequency array.
- bmp8_computeCDF transforms this histogram into a cumulative distribution function, enabling histogram equalization calculations.
- bmp8_equalize applies the equalization transform to redistribute pixel values across the full intensity range, significantly improving contrast in underexposed or overexposed images.

And for color images :
- bmp24_equalize: Converts the image to YUV color space and equalizes only the luminance (Y) channel. This method improves contrast while preserving natural color tones, making it more suitable for    
  realistic results.

  
## 3. Project Architecture

The codebase follows a modular structure:

main.c contains the command-line interface implementation and program flow control. It handles user input and coordinates function calls between modules.
bmp8.h and .c pair implements all grayscale-specific operations, including features functions, basic transformations, and filtering.
bmp24.h and .] manages color image processing, building on the grayscale functionality while adding color-specific operations.

The testing directory in the c-make-build contains test images that verify correct operation: lena_gray.bmp for grayscale testing and flowers_color.bmp for color processing validation.

## 4. User Interface Guide

The text-based interface guides users through a hierarchical menu system:

1. Primary Menu (System Level)
   - Image loading/saving
   - Program exit

- displayMainMenu():
Displays the main menu with options for loading, saving, filtering, and exiting the program.
- displayFilterMenu():
Displays a sub-menu for choosing an image filter to apply.
- bmp8_openImage():
Prompts the user for a file path and loads a grayscale (8-bit) image from disk.
- bmp24_openImage():
Prompts the user for a file path and loads a color (24-bit) image from disk.
- saveImage(int bit, void *img):
Saves either a grayscale or color image to disk based on user input and image type

2. Processing Menu (Operation Selection)
   - Filter application
   - Color adjustment
   - Histogram operations

- bmp8_handleFilterChoice(int choice, t_bmp8 *img):
Processes the selected filter option and applies the corresponding transformation to a grayscale image.
- bmp24_handleFilterChoice(int choice, t_bmp24 *img):
Processes the selected filter option and applies the corresponding transformation to a color image.
- applyFilter(int bit, void *img):
Displays the filter menu and routes the filter logic to the appropriate grayscale or color handler based on image type.
- displayImageInfo(int bit, void *img):
Displays image information (width, height, color depth, etc.) for either grayscale or color images.


3. Parameter Menus (Operation Configuration)
   - Filter type selection
   - Adjustment value input
   - File path specification

Also we have these two function that allowed us to test our work with the two types of images :
- bmp8_test():
Demonstrates and tests all filters and histogram equalization on a grayscale image using "lena_gray.bmp", saving each result.
- bmp24_test():
Demonstrates and tests all filters and histogram equalization on a color image using "lena_color.bmp", saving each result.

Navigation follows standard conventions with numbered options and clear prompts. The interface validates all input and provides error feedback for invalid selections or file operations.

## 5. Credits and Attribution

Project realised by : [Raphaël Freynet--Serre, Ethan Sauvanet, Adrien de Chavagnac]

Supervised by: Professor Rado Rakotonarivo

Institution: Efrei Paris - P1-INT1 2024/2025

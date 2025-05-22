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

The bmp24 module extends functionality to color images:

bmp24_loadImage and bmp24_saveImage handle the more complex 24-bit BMP format, managing three color channels per pixel. The loader verifies color depth and handles pixel data organization.

Color-specific transformations include:
- bmp24_grayscale: Converts RGB values to luminance using the standard weighting formula (0.299R + 0.587G + 0.114B)
- bmp24_equalize: Performs histogram equalization in YUV color space for natural-looking contrast enhancement

### 2.3 Histogram Operations

The histogram processing system consists of three core functions:

- bmp8_computeHistogram analyzes pixel value distribution across the 0-255 range, returning a 256-element frequency array.
- bmp8_computeCDF transforms this histogram into a cumulative distribution function, enabling histogram equalization calculations.
- bmp8_equalize applies the equalization transform to redistribute pixel values across the full intensity range, significantly improving contrast in underexposed or overexposed images.

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

2. Processing Menu (Operation Selection)
   - Filter application
   - Color adjustment
   - Histogram operations

3. Parameter Menus (Operation Configuration)
   - Filter type selection
   - Adjustment value input
   - File path specification

Navigation follows standard conventions with numbered options and clear prompts. The interface validates all input and provides error feedback for invalid selections or file operations.

## 5. Credits and Attribution

Project realised by : [Raphaël Freynet--Serre, Ethan Sauvanet, Adrien de Chavagnac]

Supervised by: Professor Rado Rakotonarivo

Institution: Efrei Paris - P1-INT1 2024/2025

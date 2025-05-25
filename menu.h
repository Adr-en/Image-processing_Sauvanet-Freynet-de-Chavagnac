//
// Created by adrie on 24/05/2025.
//
#ifndef MENU_H
#define MENU_H

#include "bmp8.h"
#include "bmp24.h"

extern float **box_blur ;
extern float **gaussian_blur ;
extern float **outline ;
extern float **emboss ;
extern float **sharpen ;
extern float **zero ;

void displayMainMenu();
void displayFilterMenu();
t_bmp8 * bmp8_openImage();
t_bmp24 * bmp24_openImage();
void saveImage(int bit, void * img);
void applyFilter(int bit, void *img);
void displayImageInfo(int bit, void * img);
void bmp8_handleFilterChoice(int choice, t_bmp8 *img);
void bmp24_handleFilterChoice(int choice, t_bmp24 *img);
void bmp8_test();
void bmp24_test();




#endif //MENU_H

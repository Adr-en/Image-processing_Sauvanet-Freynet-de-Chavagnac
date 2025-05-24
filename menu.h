//
// Created by adrie on 24/05/2025.
//
#include <stdio.h>
#include <stdlib.h>
#ifndef MENU_H
#define MENU_H

extern float **box_blur ;
extern float **gaussian_blur ;
extern float **outline ;
extern float **emboss ;
extern float **sharpen ;
extern float **zero ;

void displayMainMenu();
void displayFilterMenu();
void openImage();
void saveImage();
void applyFilter();
void displayImageInfo();
void bmp8_handleFilterChoice(int choice, char * filepath);
void bmp24_handleFilterChoice(int choice, char * filepath);



#endif //MENU_H

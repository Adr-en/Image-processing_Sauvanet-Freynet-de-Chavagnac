# include <stdio.h>
# include <stdlib.h>
#include "bmp8.h.c"

t_bmp8 * bmp8_loadImage(const char * filename){
  t_bmp8 * p_image = (t_bmp8 *) malloc(sizeof(t_bmp8));
  return p_image;
  }
void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);
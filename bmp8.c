# include <stdio.h>
# include <stdlib.h>
#include "bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filename){
  t_bmp8 * p_image = (t_bmp8 *) malloc(sizeof(t_bmp8));
  if (p_image == NULL){
    printf("Memory allocation error\n");
    return NULL;
  }

  return p_image;
  }


 //La prof a écrit ça au tableau
  FILE *f = fopen(filename, "rb");
  unsigned header[54];
      fread(header, 1, 54, f); //54 element of 1 byte

  int width = *(unsigned int *) (&header[18]);     //(unsigned int *) correspond to 4 bytes
  int height = *(unsigned int *) (&header[22]);    // when we fread a file the index pointed by &header is the last one (54) we have to open it again to go back to index 0








void bmp8_saveImage(const char * filename, t_bmp8 * img);





void bmp8_free(t_bmp8 * img);





void bmp8_printInfo(t_bmp8 * img);
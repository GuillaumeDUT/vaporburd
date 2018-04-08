#include "libs.h"

#include "ppm_reader.h"

/* Return the width of the map */
int createFromPPM( OList *obstacles, char *filename ) {  
  printf("PPM File : %s\n", filename);
  FILE *ppmFile;
  char line[64];
  int w, h, j, i, r, g, b, maxColorValue;
  float x, y;
  
  /* On ouvre le fichier */
  ppmFile = fopen(filename, "r");  
  if ( ppmFile == NULL ) {
    printf("Error opening the file\n");
    exit(0);
  }
  
  /* 
    Lecture de la premiere ligne
    Contient P3
  */
  fgets(line, 64, ppmFile);
  printf("line 1 : %s", line);
  if ( strcmp(line, "P3\n") != 0 ) {
    printf("Error : Not a PPM file\n");
    exit(0);
  }
  
  /* 
    Lecture de la deuxieme ligne
    Contient <width> <height>
  */
  fgets(line, 64, ppmFile);
  sscanf(line, "%d %d", &w, &h);
  /*
  printf("line 2 : %s", line);
  printf("w:%d h:%d\n", w, h);
  */
  
  /* 
    Lecture de la troisieme ligne
    Contient la valeur de couleure maximale
  */
  fgets(line, 64, ppmFile);
  sscanf(line, "%d", &maxColorValue);
  /*
  printf("line 3 : %s", line);
  printf("Max:%d\n", maxColorValue);
  */
  
  /*
    Lecture du fichier jusqu'a la fin
    Chaque ligne contient une valeure soit R, V, B    
    WIDTH = j
    HEIGHT = i
  */
  for( i=0; i<h; i++) {
    for( j=0; j<w; j++) {
      fgets(line, 64, ppmFile);
      sscanf(line, "%d", &r);
      fgets(line, 64, ppmFile);
      sscanf(line, "%d", &g);
      fgets(line, 64, ppmFile);
      sscanf(line, "%d", &b);
      
      
      /* Red */
      if ( r==255 && g==0 && b==0 ) {
        x = (float)j;
        y = (float)i*WINDOW_SCALE/(float)h - WINDOW_SCALE/2;
        ajouterFinOList(obstacles, createObstacle( x, y, 20, 1));
      }
      /* Blue */
      /*
      if ( r==0 && g==0 && b==255 ) {
        printf("BLUE (%d, %d) : %d %d %d\n", i, j, r, g, b);
      }
      */

    }
  }
  
  return w;

}
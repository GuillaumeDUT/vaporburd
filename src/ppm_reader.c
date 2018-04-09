#include "libs.h"

#include "ppm_reader.h"

/* Return the width of the map */
int createFromPPM( char *filename, OList *obstacles, BList *bonuses ) {  
  printf("PPM File : %s\n", filename);
  FILE *ppmFile;
  char line[64];
  int w, h, i, j, r, g, b, max;
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
  /*
  printf("line 1 : %s", line);
  */
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
  sscanf(line, "%d", &max);
  /*
  printf("line 3 : %s", line);
  printf("Max:%d\n", max);
  */
  
  /*
    Lecture du fichier jusqu'a la fin
    Chaque ligne contient une valeure soit R, V, B    
    WIDTH = j
    HEIGHT = i
  */
  for( i=0; i<h; i+=1) {
    for( j=0; j<w; j+=1) {
      fgets(line, 64, ppmFile);
      sscanf(line, "%d", &r);
      fgets(line, 64, ppmFile);
      sscanf(line, "%d", &g);
      fgets(line, 64, ppmFile);
      sscanf(line, "%d", &b);
      
      if ( r!=0 || g!=0 || b!=0 ) {
        x = 1.0 * j;
        y = fmap((float) i, 0, h, WINDOW_SCALE/2 - 0.5, -WINDOW_SCALE/2 - 0.5);
      
        /*printf("COLOR (%d, %d) : %d %d %d\n", i, j, r, g, b);*/
        /* Red */
        if ( r==max && g==0 && b==0 ) {
          ajouterFinList(obstacles, createObstacle( x, y, 20, 1));
        }
        /* Blue */
        if ( r==0 && g==0 && b==max ) {
          ajouterFinList(bonuses, createBonus( x, y, 1, 0.5, BONUS_TYPE_DAMAGES));
        }
        /* Green */
        if ( r==0 && g==max && b==0 ) {
          ajouterFinList(bonuses, createBonus( x, y, 1, 0.5, BONUS_TYPE_ATTACK_SPEED));
        }
        /* Cyan */
        if ( r==0 && g==max && b==max ) {
          ajouterFinList(bonuses, createBonus( x, y, 1, 0.5, BONUS_TYPE_MISSILE_UP));
        }
        /* Magenta */
        if ( r==max && g==0 && b==max ) {
          ajouterFinList(obstacles, createObstacle( x, y, 20, 1));
        }
      }
    }
  }
  
  return w;

}
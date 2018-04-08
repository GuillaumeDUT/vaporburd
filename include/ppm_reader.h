#ifndef PPM_READER_H
#define PPM_READER_H

#include "obstacle.h"

typedef struct ppmnode {
  int x, y, time;
  struct node *next;
} *PPMNode;

int createFromPPM( OList *obstacles, char *filename );

#endif

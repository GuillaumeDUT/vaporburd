#ifndef PPM_READER_H
#define PPM_READER_H

#include "obstacle.h"

typedef struct ppmnode {
  int x, y, time;
  struct node *next;
} *PPMNode;

void createFromPPM( OList *obstacles, char *filename );

#endif

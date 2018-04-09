#ifndef PPM_READER_H
#define PPM_READER_H

#include "obstacle.h"
#include "lists.h"
#include "bonus.h"

int createFromPPM( char *filename, OList *obstacles, BList *bonuses );

#endif

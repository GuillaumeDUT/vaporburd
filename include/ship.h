#ifndef SHIP_H
#define SHIP_H

#include "constants.h"
#include "entities.h"
#include "helpers.h"


void moveUp(Ship ship);
void moveDown(Ship ship);
void moveLeft(Ship ship);
void moveRight(Ship ship);

void moveShip(Ship ship, float globalTranslation, float globalTranslationTotal, int slow);

void drawShip(Ship ship, int full);

#endif

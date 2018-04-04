#ifndef SHIP_H
#define SHIP_H

#include "entities.h"
#include "helpers.h"

#define ACC 0.027
#define MOMENTUM 0.85

void moveUp(Ship ship);
void moveDown(Ship ship);
void moveLeft(Ship ship);
void moveRight(Ship ship);

void moveShip(Ship ship);

void displayShip(Ship ship);

#endif

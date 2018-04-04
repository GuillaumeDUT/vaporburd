#ifndef SHIP_H
#define SHIP_H

#define ACC 0.015
#define MOMENTUM 0.9

void moveUp(Ship ship);
void moveDown(Ship ship);
void moveLeft(Ship ship);
void moveRight(Ship ship);

void moveShip(Ship ship);

void displayShip(Ship ship);
#endif

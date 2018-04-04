#ifndef SHIP_H
#define SHIP_H

//drawShip


typedef struct Entity{
	float x;
	float y;
	int hp;
}*Ship, *Ennemy, *Obstacle;

Ship createShip(float x, float y, int hp);
Ennemy createEnnemy(float x, float y, int hp);
Obstacle createObstacle(float x, float y, int hp);

void drawShip(Ship ship);

#endif

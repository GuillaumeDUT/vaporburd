#ifndef ENTITIES_H
#define ENTITIES_H
#define X 0
#define Y 1

#include "basic_shape.h"

typedef struct Entity{
	float pos[2];
	float speed[2];
	int hp;
	int id;
	float size;
	float min[2];
	float max[2];
	struct Entity *next;
	struct Entity *before;
}*Ship, *Ennemy, *Obstacle;

Ship createShip(float x, float y, int hp, float size);
Ennemy createEnnemy(float x, float y, int hp, float size);
Obstacle createObstacle(float x, float y, int hp, float size);

void drawShip(Ship ship);

#endif

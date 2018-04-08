#ifndef ENTITIES_H
#define ENTITIES_H

#include "basic_shape.h"
#include "constants.h"

typedef struct Entity{
	int hp;
	int id;
	float size;
	int damages;
	float pos[2];
	float speed[2];
	float min[2];
	float max[2];
	struct Entity *next;
	struct Entity *before;
} *Ship, *Ennemy, *Obstacle, *Bullet, *Entity;

Ship createShip(float x, float y, int hp, float size);
Ennemy createEnnemy(float x, float y, int hp, float size);
Obstacle createObstacle(float x, float y, int hp, float size);
Bullet createBullet(Ship ship);

Entity createEntity(float x, float y, int hp, float size, int id);

#endif

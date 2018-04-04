#ifndef ENTITIES_H
#define ENTITIES_H

#define X 0
#define Y 1

typedef struct Entity{
	float pos[2];
	float speed[2];
	int hp;
}*Ship, *Ennemy, *Obstacle;

Ship createShip(float x, float y, int hp);
Ennemy createEnnemy(float x, float y, int hp);
Obstacle createObstacle(float x, float y, int hp);

void drawShip(Ship ship);

#endif

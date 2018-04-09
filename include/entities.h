#ifndef ENTITIES_H
#define ENTITIES_H

#include "basic_shape.h"
#include "constants.h"

typedef struct Entity{
	int hp;
	int id;  
	float size;
  
	float pos[2];
	float speed[2];
	float min[2];
	float max[2];
  
  int bonusType;
	int damages;
  int attackSpeed;
  
	struct Entity *next;
	struct Entity *before;
} *Ship, *Ennemy, *Obstacle, *Bullet, *Entity, *Bonus;

Ship createShip(float x, float y, int hp, float size);
Ennemy createEnnemy(float x, float y, int hp, float size);
Obstacle createObstacle(float x, float y, int hp, float size);
Bonus createBonus(float x, float y, int hp, float size, int type);
Bullet createBullet(Ship ship);

Entity createEntity(float x, float y, int hp, float size, int id);

void displayEntity(Entity e);
/* 
  Return :
  0: Dead
  1: Alive
*/
void getDamage(Entity attacker, Entity defender);


#endif

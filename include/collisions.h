#ifndef COLLISION_H
#define COLLISION_H

#include "entities.h"
#include "ship.h"
#include "shoot.h"
#include "obstacle.h"

int collisionBulletEnnemy(Bullet b, Ennemy e);
int collisionShipEnnemy(Ship s, Ennemy e);
int collisionShipObstacle(Ship s, Obstacle o);

#endif

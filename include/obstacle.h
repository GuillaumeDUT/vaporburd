#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entities.h"
#include "ship.h"
#include "lists.h"
#include "collisions.h"

void updateObstacles(Ship ship, OList *liste);
void drawObstacle(Obstacle obstacle, int full);

#endif

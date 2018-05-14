#ifndef BONUS_H
#define BONUS_H

#include "entities.h"
#include "constants.h"
#include "basic_shape.h"
#include "lists.h"
#include "ship.h"
#include "collisions.h"

void drawBonus(Bonus b, int full,GLuint textureID[]);
void updateBonuses(Ship ship, BList *bonuses,GLuint textureID[]);
void acquireBonus(Ship ship, Bonus bonus);

#endif

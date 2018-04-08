#ifndef ENNEMIES_H
#define ENNEMIES_H

#include "entities.h"
#include "helpers.h"
#include "lists.h"
#include "shoot.h"
#include "ship.h"
#include "collisions.h"

void newRandomEnnemy(EList *ennemiesList, float globalTranslation);
void loopEList(Ship ship, BList *bullets, EList *liste);
void drawEnnemy(Ennemy ennemy, int full);

#endif

#ifndef ENNEMIES_H
#define ENNEMIES_H

#include "entities.h"
#include "osu_reader.h"
#include "helpers.h"
#include "lists.h"
#include "shoot.h"
#include "ship.h"
#include "collisions.h"

void newRandomEnnemy(EList *ennemies, float globalTranslation);
void newOSUNodeEnnemy(EList *ennemies, OSUNode oNode, float globalTranslation);
void updateEnnemies(Ship ship, BList *bullets, EList *liste);
void drawEnnemy(Ennemy ennemy, int full);

#endif

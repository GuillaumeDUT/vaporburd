#ifndef ENNEMIES_H
#define ENNEMIES_H

#include "entities.h"
#include "osu_reader.h"
#include "helpers.h"
#include "lists.h"
#include "shoot.h"
#include "ship.h"
#include "collisions.h"

void createRandomEnnemy(EList *ennemies, float globalTranslation);
void createOSUNodeEnnemy(EList *ennemies, OSUNode oNode, float globalTranslation);
void updateEnnemies(Ship ship, BList *bullets, EList *liste, int globalTranslationTotal,GLuint textureID[]);
void drawEnnemy(Ennemy ennemy, int full,GLuint textureID[]);
void moveEnnemy(Ennemy ennemy, int globalTranslationTotal);
void createBoss(EList *ennemies, float globalTranslation, float globalTranslationTotal);
void shootEnnemy(Ennemy ennemy, BList *bullets);
void bossPattern1(Ennemy e, Ship s, BList *bullets);

#endif

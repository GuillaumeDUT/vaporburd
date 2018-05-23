#ifndef ENNEMIES_H
#define ENNEMIES_H

#include "entities.h"
#include "osu_reader.h"
#include "helpers.h"
#include "lists.h"
#include "shoot.h"
#include "ship.h"
#include "collisions.h"

/* Ennemy creation */
void createRandomEnnemy(EList *ennemies, float globalTranslation);
void createOSUNodeEnnemy(EList *ennemies, OSUNode oNode, float globalTranslation);
void createHommingEnnemy(EList *ennemies, float y, Ennemy boss);
void createBoss(EList *ennemies, float globalTranslation, float globalTranslationTotal);



/* Update Loop */
void updateEnnemies(Ship ship, BList *bulletsEnnemy, BList *bulletsShip, EList *liste, float globalTranslationTotal, GLuint textureID[]);
void drawEnnemy(Ennemy ennemy, int full,GLuint textureID[]);
void moveEnnemy(Ennemy ennemy, int globalTranslationTotal);
void shootEnnemy(Ennemy ennemy, BList *bullets);


/* Boss pattern */
void bossPattern1(Ennemy boss, Ship s, BList *bullets);
void bossPattern2(Ennemy boss, Ship s, BList *bullets, EList *ennemies);
void bossPattern3(Ennemy boss, Ship s, BList *bullets);
void bossPattern4(Ennemy boss, Ship s, BList *bullets);
void bossPattern5(Ennemy boss, Ship s, BList *bullets);

#endif

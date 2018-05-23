#ifndef SHOOT_H
#define SHOOT_H

#include "constants.h"
#include "entities.h"
#include "lists.h"
#include "helpers.h"

// tire en fonction de la position actuelle du vaisseau
void shoot(Entity entity, BList *liste);
void updateBullets(Ship ship, BList *liste, float globalTranslationTotal, GLuint textureID[]);
void drawBullet( Bullet bullet, GLuint textureID[]);
void moveBullet( Bullet bullet );
void bossAttack1(Entity boss, Ship ship, BList *bList, int deltaTime);
void bossAttack3(Entity boss, Ship ship, BList *bList);

#endif

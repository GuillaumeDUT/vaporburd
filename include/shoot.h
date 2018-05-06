#ifndef SHOOT_H
#define SHOOT_H

#include "constants.h"
#include "entities.h"
#include "lists.h"
#include "helpers.h"

// tire en fonction de la position actuelle du vaisseau
void shoot(Entity entity, BList *liste);
void updateBullets(Ship ship, BList *liste, float globalTranslationTotal);
void drawBullet( Bullet bullet );
void moveBullet( Bullet bullet );
void bossAttack1(Entity entity, Ship ship, BList *liste, int deltaTime);

#endif

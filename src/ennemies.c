#include "libs.h"

#include "ennemies.h"

void newRandomEnnemy(EList *ennemies, float globalTranslation) {
  int limit = WINDOW_SCALE/2 - 1;
  ajouterFinList(ennemies, createEnnemy(
    frand_a_b(limit, limit-5) + globalTranslation,
    frand_a_b(-limit, limit),
    5, /* HP */
    0.5 /* SIZE */));
}

void loopEList(Ship ship, BList *bullets, EList *ennemies) {
  if(ennemies->taille == 0){
    return ;
  }
  Ennemy eActuel = ennemies->first;
  Ennemy eNext;
  while ( eActuel != NULL ) {
    eNext = eActuel->next;
    /*
    moveEnnemy( eActuel );
    */
    drawEnnemy( eActuel, 0 );
    
    
    /* Collision avec les bullets */
    if(bullets->taille != 0){
      Bullet bulletActuel = bullets->first;
      Bullet bulletNext;
      while ( bulletActuel != NULL ) {
        bulletNext = bulletActuel->next;
        if ( collision(bulletActuel, eActuel) ) {
          drawEnnemy( eActuel, 1 );
          getDamage(bulletActuel, eActuel);
          supprimerList(bullets, bulletActuel->id);
        }
        bulletActuel = bulletNext;
      }
    }
    
    /* Collision avec le ship */
    if ( collision(ship, eActuel) ) {
			drawShip(ship, 1);
			drawEnnemy(eActuel, 1);
      getDamage(ship, eActuel);
      getDamage(eActuel, ship);
      displayEntity(ship);
		}
    
    if ( eActuel->hp <= 0 )  {
      supprimerList(ennemies, eActuel->id);
    }
    eActuel = eNext;
  }
}
void drawEnnemy( Ennemy ennemy, int full ) {
  glPushMatrix();
  glColor3f(255, 0, 120);
  glTranslatef(
    ennemy->pos[X],
    ennemy->pos[Y],
    0);
  glScalef(
    ennemy->size,
    ennemy->size,
    1);
  drawCircle(full);
  glPopMatrix();
}

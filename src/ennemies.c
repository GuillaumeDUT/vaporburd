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
  Ennemy ennemyActuel = ennemies->first;
  Ennemy next;
  while ( ennemyActuel != NULL ) {
    next = ennemyActuel->next;
    /*
    moveEnnemy( ennemyActuel );
    */
    drawEnnemy( ennemyActuel, 0 );
    
    
    /* Collision avec les bullets */
    if(bullets->taille != 0){
      Bullet bulletActuel = bullets->first;
      while ( bulletActuel != NULL ) {
        if ( collision(bulletActuel, ennemyActuel) ) {
          drawEnnemy( ennemyActuel, 1 );
          ennemyActuel->hp -= BULLET_DAMAGES;
        }
        bulletActuel = bulletActuel->next;
      }
    }
    
    /* Collision avec le ship */
    if ( collision(ship, ennemyActuel) ) {
			drawShip(ship, 1);
			drawEnnemy(ennemyActuel, 1);
		}
    
    if ( ennemyActuel->hp <= 0 )  {
      supprimerList(ennemies, ennemyActuel->id);
    }
    
    
    ennemyActuel = next;
  }
}
void drawEnnemy( Ennemy ennemy, int full ) {
//  printf("(%d) posX:%f posY:%f\n", ennemy->id, ennemy->pos[X], ennemy->pos[Y]);
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

#include "libs.h"

#include "bonus.h"

void drawBonus(Bonus b, int full) {
  glPushMatrix(); {
    glColor3f(0, 0, 255);
    glTranslatef(b->pos[X], b->pos[Y], 0);
    glScalef(b->size, b->size, 1);
    drawCircle(full);
  } glPopMatrix();
}

void updateBonuses(Ship ship, BList *bonuses) {
  if(bonuses->taille == 0){
    return ;
  }
  Bonus bActuel = bonuses->first;
  Bonus bNext;
  while ( bActuel != NULL ) {
    bNext = bActuel->next;
    
    /* Affichage */
    drawBonus(bActuel, 0);
    
    /* Collision avec le ship */
    if ( collision(ship, bActuel) ) {
      drawShip(ship, 1);
      drawBonus(bActuel, 1);
      printf("Bonus\n");
      acquireBonus(ship, bActuel);
      supprimerList(bonuses, bActuel->id);
    }
    bActuel = bNext;
  }
}

void acquireBonus(Ship ship, Bonus bonus) {
  switch(bonus->bonusType) {
    case BONUS_TYPE_DAMAGES:
      ship->damages += SHIP_DAMAGES;
      break;
    case BONUS_TYPE_ATTACK_SPEED:
      ship->attackPerSecond += SHIP_ATTACK_SPEED;
      break;
    case BONUS_TYPE_MISSILE_UP:
      ship->missileLevel += 1;
      break;
    default:
      break;
  }
}
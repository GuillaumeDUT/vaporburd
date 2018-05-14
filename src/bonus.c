#include "libs.h"

#include "bonus.h"

void drawBonus(Bonus b, int full,GLuint textureID[]) {
  glPushMatrix(); {
    glColor3f(0, 0, 255);
    glTranslatef(b->pos[X], b->pos[Y], 0);

    glScalef(b->size, b->size, 1);
    glEnable(GL_TEXTURE_2D);

    if(b->bonusType == BONUS_TYPE_DAMAGES){
    
      glBindTexture(GL_TEXTURE_2D, textureID[5]);

    }else if(b->bonusType == BONUS_TYPE_ATTACK_SPEED){

      glBindTexture(GL_TEXTURE_2D, textureID[6]);

    }else if(b->bonusType == BONUS_TYPE_MISSILE_UP){

      glBindTexture(GL_TEXTURE_2D, textureID[7]);
    }
    glBegin(GL_QUADS);
    {
      glColor3ub(255,255,255);

      glTexCoord2f(0, 0);
      glVertex2f(-1, +1);

      glTexCoord2f(1, 0);
      glVertex2f(+1, +1);

      glTexCoord2f(1, 1);
      glVertex2f(+1, -1);

      glTexCoord2f(0, 1);
      glVertex2f(-1, -1);
    }

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    drawCircle(full);
  } glPopMatrix();
}

void updateBonuses(Ship ship, BList *bonuses,GLuint textureID[]) {
  if(bonuses->taille == 0){
    return ;
  }
  Bonus bActuel = bonuses->first;
  Bonus bNext;
  while ( bActuel != NULL ) {
    bNext = bActuel->next;
    
    /* Affichage */
    drawBonus(bActuel, 0, textureID);
    
    /* Collision avec le ship */
    if ( collision(ship, bActuel) ) {
      drawShip(ship, 1);
      drawBonus(bActuel, 1, textureID);
      printf("Bonus\n");
      acquireBonus(ship, bActuel);
      supprimerList(bonuses, bActuel->id);
    }
    bActuel = bNext;
  }
}

void acquireBonus(Ship ship, Bonus bonus) {
  switch(bonus->type) {
    case BONUS_TYPE_DAMAGE:
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
#include "libs.h"

#include "shoot.h"

void shoot(Entity entity, BList *liste){
  int max = intMin(entity->missileLevel, 5);
  Bullet bullets[max];
  int i;

  for ( i=0; i<max; i++ ) {
    bullets[i] = createBullet(entity, BULLET_SIZE);
    ajouterFinList(liste, bullets[i]);
  }

  switch (max) {
    case 2:
      bullets[0]->speed[Y] = -0.02;
      bullets[1]->speed[Y] = 0.02;
      break;
    case 3:
      bullets[1]->speed[Y] = -0.02;
      bullets[2]->speed[Y] = 0.02;
      break;
    case 4: {
      Bullet b5 = createBullet(entity, BULLET_SIZE);
      ajouterFinList(liste, b5);
      float size = b5->size;
      bullets[1]->pos[Y] -= size * 4;
      bullets[2]->pos[Y] -= size * 2;
      bullets[3]->pos[Y] += size * 2;
      b5->pos[Y] += size * 4;
    }
      break;
    case 5: {
      float size =  bullets[1]->size;
      bullets[1]->pos[Y] -= size * 4;
      bullets[2]->pos[Y] -= size * 2;
      bullets[3]->pos[Y] += size * 2;
      bullets[4]->pos[Y] += size * 4;
    }
      break;
    default:
      break;
  }
}
void updateBullets(Ship ship, BList *liste, float globalTranslationTotal,GLuint textureID[]) {
  if(liste->taille == 0){
    return ;
  }
  Bullet actuel = liste->first;
  Bullet next;
  while ( actuel != NULL ) {
    next = actuel->next;
    moveBullet( actuel );
    drawBullet( actuel, textureID);

    /* Supprime quand on sort de l'ecran */
    if ( actuel->pos[X] >= WINDOW_SCALE / 2 + globalTranslationTotal ) {
      supprimerList( liste, actuel->id );
    }
    /* Next */
    actuel = next;
  }
}
void drawBullet( Bullet bullet,GLuint textureID[]) {
  //  printf("(%d) posX:%f posY:%f\n", bullet->id, bullet->pos[X], bullet->pos[Y]);

  glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    if(bullet->ennemyType == ENNEMY_TYPE_BASIC){
        glBindTexture(GL_TEXTURE_2D, textureID[4]);
     
        glBegin(GL_QUADS);
        {
          glColor3ub(255,255,255);
          glScalef(0.5,0.5,1);
          glTexCoord2f(0, 0);
          glVertex2f(bullet->pos[X]-0.5, bullet->pos[Y]+0.5);

          glTexCoord2f(1, 0);
          glVertex2f(bullet->pos[X]+0.5, bullet->pos[Y]+0.5);

          glTexCoord2f(1, 1);
          glVertex2f(bullet->pos[X]+0.5, bullet->pos[Y]-0.5);

          glTexCoord2f(0, 1);
          glVertex2f(bullet->pos[X]-0.5, bullet->pos[Y]-0.5);
        }
        glEnd();
      }else if(bullet->ennemyType == NOT_AN_ENNEMY){
      glBindTexture(GL_TEXTURE_2D, textureID[1]);
        glBegin(GL_QUADS);
        {
          glColor3ub(255,255,255);

          glTexCoord2f(0, 0);
          glVertex2f(bullet->pos[X]-1.45, bullet->pos[Y]+0.5);

          glTexCoord2f(1, 0);
          glVertex2f(bullet->pos[X]+1.45, bullet->pos[Y]+0.5);

          glTexCoord2f(1, 1);
          glVertex2f(bullet->pos[X]+1.45, bullet->pos[Y]-0.5);

          glTexCoord2f(0, 1);
          glVertex2f(bullet->pos[X]-1.45, bullet->pos[Y]-0.5);
        }
        glEnd();
    }
        //glColor3f(0, 155, 155);
    glTranslatef(
      bullet->pos[X],
      bullet->pos[Y],
      0);
    glScalef(
      bullet->size,
      bullet->size,
      1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    drawCircle(1);
  glPopMatrix();
}
void moveBullet( Bullet bullet ) {
  bullet->pos[X] += bullet->speed[X];
  bullet->pos[Y] += bullet->speed[Y];
}

void bossAttack1(Entity entity, Ship ship, BList *liste, int deltaTime) {
  float deltaX, deltaY;
  
  Bullet bullet = createBullet(entity, 2);
  ajouterFinList(liste, bullet);

  bullet->pos[Y] += (deltaTime-25) / 3;

  deltaX = ship->pos[X] - bullet->pos[X];
  deltaY = ship->pos[Y] - bullet->pos[Y];

  bullet->speed[Y] = deltaY/300;
  bullet->speed[X] = deltaX/300;
  
  bullet->speed[Y] = 0;
  bullet->speed[X] = -0.1;
}

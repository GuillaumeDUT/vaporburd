#include "libs.h"
#include "constants.h"

#include "shoot.h"

void shoot(Ship ship, BList *liste){
  Bullet bullet = createBullet( ship );
  ajouterFinList(liste, bullet);
  return;
}
void loopBList(Ship ship, BList *liste, float globalTranslationTotal) {
  if(liste->taille == 0){
    return ;
  }
  Bullet actuel = liste->first;
  Bullet next;
  while ( actuel != NULL ) {
    next = actuel->next;
    moveBullet( actuel );
    drawBullet( actuel );
    
    /* Supprime quand on sort de l'ecran */
    if ( actuel->pos[X] >= WINDOW_SCALE / 2 + globalTranslationTotal ) {
      supprimerList( liste, actuel->id );
    }
    /* Next */
    actuel = next;
  }
}
void drawBullet( Bullet bullet ) {
//  printf("(%d) posX:%f posY:%f\n", bullet->id, bullet->pos[X], bullet->pos[Y]);

  glPushMatrix();
  glColor3f(0, 155, 155);
  glTranslatef(
    bullet->pos[X],
    bullet->pos[Y],
    0);
  glScalef(
    bullet->size,
    bullet->size,
    1);
  drawCircle(1);
  glPopMatrix();
}
void moveBullet( Bullet bullet ) {
  bullet->pos[X] += BULLET_SPEED;
}


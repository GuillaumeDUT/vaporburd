#include "libs.h"

#include "ship.h"

/* Récupère et applique les evenements du clavier */
void moveUp(Ship ship) {
  if ( !ship ) {
    printf("The ship is undefined.\n");
    return;
  }
  ship->speed[Y] += ACC;
}
void moveDown(Ship ship) {
  if ( !ship ) {
    printf("The ship is undefined.\n");
    return;
  }
  ship->speed[Y] -= ACC;
}
void moveLeft(Ship ship) {
  if ( !ship ) {
    printf("The ship is undefined.\n");
    return;
  }
  ship->speed[X] -= ACC;
}
void moveRight(Ship ship) {
  if ( !ship ) {
    printf("The ship is undefined.\n");
    return;
  }
  ship->speed[X] += ACC;
}

/* Déplace le vaisson en fonction de son vecteur de vitesse */
void moveShip(Ship ship, float globalTranslation, float globalTranslationTotal, int slow) {
  int LIMIT = WINDOW_SCALE/2;
  float multiplier = slow ? 0.6 : 1;

  ship->pos[X] += ship->speed[X];
  ship->pos[Y] += ship->speed[Y];

  ship->speed[X] = absFloat(ship->speed[X]) > 0.0001 ? ship->speed[X] * MOMENTUM * multiplier : 0;
  ship->speed[Y] = absFloat(ship->speed[Y]) > 0.0001 ? ship->speed[Y] * MOMENTUM * multiplier : 0;

  /* Fix */
  ship->pos[X] += globalTranslation;

  /* Deplacement du bas en haut */
  if ( ship->pos[Y] > LIMIT ) ship->pos[Y] -= WINDOW_SCALE;
  if ( ship->pos[Y] < -LIMIT ) ship->pos[Y] += WINDOW_SCALE;

  /* Constrain */
  if ( ship->pos[X] < -LIMIT + globalTranslationTotal ) {
    ship->pos[X] = -LIMIT + globalTranslationTotal;
    ship->speed[X] = 0;
  }
  if ( ship->pos[X] > LIMIT + globalTranslationTotal) {
    ship->pos[X] = LIMIT + globalTranslationTotal;
    ship->speed[X] = 0;
  }
}

void drawShip(Ship ship, int full){

  glPushMatrix();
  glColor3f(255, 255, 255);
  glTranslatef(
    ship->pos[X],
    ship->pos[Y],
    0);
  glScalef(
    ship->size,
    ship->size,
    1);
  drawCircle(full);
  glPopMatrix();

  /* Indicateurs de la bounding box */
  /* Min : vert */
  glPushMatrix();
  glColor3f(0, 0, 255);
  glTranslatef(
    ship->pos[X] + ship->min[X],
    ship->pos[Y] + ship->min[Y],
    0);
  glScalef(
    ship->size / 10,
    ship->size / 10,
    1);
  drawCircle(1);
  glPopMatrix();

  /* Max : rouge */
  glPushMatrix();
  glColor3f(255, 0, 0);
  glTranslatef(
    ship->pos[X] + ship->max[X],
    ship->pos[Y] + ship->max[Y],
    0);
  glScalef(
    ship->size / 10,
    ship->size / 10,
    1);
  drawCircle(1);
  glPopMatrix();

  /* Les HP */
  /* Affichage de la barre de vie */
  float percent = (float)ship->hp/30; // 30 is the nmax HP
  glPushMatrix(); 
  {
    glLoadIdentity();
    glColor3ub(203, 234, 131);

    /* Outline */
    glTranslatef(0, -0.92, 0);
    glScalef(1.9, 0.02, 1);

    glPushMatrix();
    {
      /* Plain */
      glTranslatef(-(1-percent)/2, 0, 0);
      glScalef(percent, 1, 1);
      drawSquare(1);
    }
    glPopMatrix();

    drawSquare(0);
  }
  glPopMatrix();

}

void updateShip(Ship ship, BList *bullets, float globalTranslation, float globalTranslationTotal, int slow) {
  moveShip(ship, globalTranslation, globalTranslationTotal, slow); 
  drawShip(ship, 0);


  /* Collision avec les bullets de l'ennemy */
  if(bullets->taille != 0){
    Bullet bulletActuel = bullets->first;
    Bullet bulletNext;
    while ( bulletActuel != NULL ) {
      bulletNext = bulletActuel->next;
      if ( bulletActuel->type == BULLET_ENNEMY && collision(bulletActuel, ship) ) {
        drawShip( ship, 1 );
        printf("Aie\n");
        getDamage(bulletActuel, ship);
        supprimerList(bullets, bulletActuel->id);
      }
      bulletActuel = bulletNext;
    }
  }
}



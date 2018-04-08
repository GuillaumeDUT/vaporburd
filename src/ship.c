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
void moveShip(Ship ship, float globalTranslation, float globalTranslationTotal) {
  int LIMIT = WINDOW_SCALE/2;
  ship->pos[X] += ship->speed[X];
  ship->pos[Y] += ship->speed[Y];

  ship->speed[X] = absFloat(ship->speed[X]) > 0.0001 ? ship->speed[X] * MOMENTUM : 0;
  ship->speed[Y] = absFloat(ship->speed[Y]) > 0.0001 ? ship->speed[Y] * MOMENTUM : 0;
  
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

void displayShip(Ship ship) {
  printf("(hp:%d) x:%f | y:%f ||| speed: x:%f, | y:%f\n", ship->hp, ship->pos[X], ship->pos[Y], ship->speed[X], ship->speed[Y] );
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

}

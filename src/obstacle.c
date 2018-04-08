#include "libs.h"

#include "obstacle.h"



/* Affiche et detecte les collisions */
void loopOList(Ship ship, OList *obstacles) {
  if(obstacles->taille == 0){
    return ;
  }
  Obstacle oActuel = obstacles->first;
  Obstacle oNext;
  while ( oActuel != NULL ) {
    oNext = oActuel->next;
    drawObstacle( oActuel, 0 );
    
    /* Collision avec le ship */
    if ( collision(ship, oActuel) ) {
      drawShip(ship, 1);
      drawObstacle(oActuel, 1);
      getDamage(ship, oActuel);
      getDamage(oActuel, ship);
      displayEntity(ship);
    }

    if ( oActuel->hp <= 0 )  {
      supprimerList(obstacles, oActuel->id);
    }
    oActuel = oNext;
  }
}
void drawObstacle(Obstacle obstacle, int full) {	
  glPushMatrix(); {
    glColor3f(255, 0, 0);
    glTranslatef(obstacle->pos[X], obstacle->pos[Y], 0);
    glScalef(obstacle->size, obstacle->size, 1);
    drawSquare(full);
  }  glPopMatrix();
}

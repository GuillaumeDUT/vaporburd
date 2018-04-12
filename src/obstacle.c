#include "libs.h"

#include "obstacle.h"



/* Affiche et detecte les collisions */
void updateObstacles(Ship ship, OList *obstacles) {
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
      if ( oActuel->endOfLevel == 0 ) {
        /* Normal obstacle */
        drawShip(ship, 1);
        drawObstacle(oActuel, 1);
        getDamage(ship, oActuel);
        getDamage(oActuel, ship);
        displayEntity(ship);
      } else if ( oActuel->endOfLevel == 1 ) {
        /* End of the level */
        printf("End of the LEVEL");
        
      }
    }

    if ( oActuel->hp <= 0 )  {
      supprimerList(obstacles, oActuel->id);
    }
    oActuel = oNext;
  }
}
void drawObstacle(Obstacle obstacle, int full) {
  glPushMatrix(); {
    if ( obstacle->endOfLevel == 0 ) {
      glColor3f(255, 0, 0);
    } else if ( obstacle->endOfLevel == 1 ) {
      glColor3f(255, 0, 255);    
    }
    glTranslatef(obstacle->pos[X], obstacle->pos[Y], 0);
    glScalef(obstacle->size, obstacle->size, 1);
    drawSquare(full);
  } glPopMatrix();
}

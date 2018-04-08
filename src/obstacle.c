#include "libs.h"

#include "obstacle.h"



/* Affiche et detecte les collisions */
void loopOList(Ship ship, OList *liste) {
	Obstacle actuel = liste->first;
  while ( actuel != NULL ) {		
		drawObstacle(actuel, 0);
		if ( collision(ship, actuel) ) {
			drawObstacle(actuel, 1);
			drawShip(ship, 1);
		}
    actuel = actuel->next;
  }
}
void drawObstacle(Obstacle obstacle, int full) {	
  glPushMatrix();
		glColor3f(255, 0, 0);
    glTranslatef(obstacle->pos[X], obstacle->pos[Y], 0);
		glScalef(obstacle->size, obstacle->size, 1);
    drawSquare(full);
  glPopMatrix();
}

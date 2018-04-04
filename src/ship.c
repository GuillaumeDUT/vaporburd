#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
void moveShip(Ship ship) {
		ship->pos[X] += ship->speed[X];
		ship->pos[Y] += ship->speed[Y];

		ship->speed[X] = absFloat(ship->speed[X]) > 0.0001 ? ship->speed[X] * MOMENTUM : 0;
		ship->speed[Y] = absFloat(ship->speed[Y]) > 0.0001 ? ship->speed[Y] * MOMENTUM : 0;

		/* Deplacement du bas en haut */
		if ( ship->pos[Y] > 5 ) ship->pos[Y] -= 10;
		if ( ship->pos[Y] < -5 ) ship->pos[Y] += 10;

		/* Constrain */
		if ( ship->pos[X] < -5 ) {
				ship->pos[X] = -5;
				ship->speed[X] = 0;
		}
		if ( ship->pos[X] > 5 ) {
				ship->pos[X] = 5;
				ship->speed[X] = 0;
		}
}

void displayShip(Ship ship) {
		printf("(hp:%d) x:%f | y:%f ||| speed: x:%f, | y:%f\n", ship->hp, ship->pos[X], ship->pos[Y], ship->speed[X], ship->speed[Y] );
}
void drawShip(Ship ship){

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
    drawCircle(1);
  glPopMatrix();
		
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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "entities.h"

Ship createShip(float x, float y, int hp){
 		Ship  temp = malloc( sizeof( struct Entity ) );
		if( !temp){
			printf("Erreur d'allocation\n");
			exit(0);
		}
		temp->pos[X] = x;
		temp->pos[Y] = y;

    temp->speed[X] = 0;
    temp->speed[Y] = 0;

    temp->hp = hp;

		return temp;
}
Ennemy createEnnemy(float x, float y, int hp){
 		Ennemy  temp = malloc( sizeof( struct Entity ) );
		if( !temp){
			printf("Erreur d'allocation\n");
			exit(0);
		}
		temp->pos[X] = x;
		temp->pos[Y] = y;

    temp->speed[X] = 0;
    temp->speed[Y] = 0;

    temp->hp = hp;

		return temp;
}
Obstacle createObstacle(float x, float y, int hp){
 		Obstacle temp = malloc( sizeof( struct Entity ) );
		if( !temp){
			printf("Erreur d'allocation\n");
			exit(0);
		}
		temp->pos[X] = x;
		temp->pos[Y] = y;

    temp->speed[X] = 0;
    temp->speed[Y] = 0;

    temp->hp = hp;

		return temp;
}

void drawShip(Ship ship){

  glPushMatrix();
		glColor3f(255, 255, 255);
    glTranslatef(ship->pos[X],ship->pos[Y],0);
    glScalef(0.5,0.5,1);
    drawCircle(1);
  glPopMatrix();

}
void drawObstacle(Obstacle obstacle) {
	
  glPushMatrix();
		glColor3f(255, 0, 0);
    glTranslatef(obstacle->pos[X],obstacle->pos[Y],0);
    glScalef(1,1,1);
    drawSquare(0);
  glPopMatrix();
}
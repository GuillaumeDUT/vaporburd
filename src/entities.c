#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

#include "entities.h"
#include "basic_shape.h"

#define X 0
#define Y 1

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

    temp->acc[X] = 0;
    temp->acc[Y] = 0;

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

    temp->acc[X] = 0;
    temp->acc[Y] = 0;

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

    temp->acc[X] = 0;
    temp->acc[Y] = 0;

    temp->hp = hp;

		return temp;
}

void drawShip(Ship ship){

  glPushMatrix();
    glTranslatef(ship->pos[X],0,0);
    glScalef(0.5,0.5,1);
    drawCircle(1);
  glPopMatrix();

}

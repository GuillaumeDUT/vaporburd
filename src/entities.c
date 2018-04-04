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



Ship* createShip(float x, float y, int hp){
 		Ship * temp = malloc( sizeof( struct Entity ) );
		if( !temp){
			printf("Erreur d'allocation\n");
			exit(0);
		}
		temp->x = x;
		temp->y = y;
		temp->hp = hp;
		return temp;
}
Ennemy* createEnnemy(float x, float y, int hp){
 		Ennemy * temp = malloc( sizeof( struct Entity ) );
		if( !temp){
			printf("Erreur d'allocation\n");
			exit(0);
		}
		temp->x = x;
		temp->y = y;
		temp->hp = hp;
		return temp;
}
Obstacle* createObstacle(float x, float y, int hp){
 		Obstacle * temp = malloc( sizeof( struct Entity ) );
		if( !temp){
			printf("Erreur d'allocation\n");
			exit(0);
		}
		temp->x = x;
		temp->y = y;
		temp->hp = hp;
		return temp;
}

void drawShip(float posX){
	//printf("PosX : %f",posX);
	glPushMatrix();
		glTranslatef(-4.0,posX,0.0);
		glScalef(0.2,0.2,0);
		drawCircle(1);
	glPopMatrix();
}

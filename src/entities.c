#include "libs.h"
#include "entities.h"

static int ennemyID = 1;
static int obstacleID = 1;

Ship createShip(float x, float y, int hp, float size){
	Ship temp = (Ship) malloc( sizeof( struct Entity ) );
	if( !temp){
		printf("Erreur d'allocation\n");
		exit(0);
	}
	temp->pos[X] = x;
	temp->pos[Y] = y;

	temp->speed[X] = 0;
	temp->speed[Y] = 0;

	temp->hp = hp;
	temp->size = size;
	temp->id = 0;
	
	temp->min[X] = -size/2;
	temp->min[Y] = -size/2;
	temp->max[X] = size/2;
	temp->max[Y] = size/2;	
	
	temp->next = NULL;
	temp->before = NULL;

	return temp;
}
Ennemy createEnnemy(float x, float y, int hp, float size){
	Ennemy temp = (Ennemy) malloc( sizeof( struct Entity ) );
	if( !temp){
		printf("Erreur d'allocation\n");
		exit(0);
	}
	temp->pos[X] = x;
	temp->pos[Y] = y;

	temp->speed[X] = 0;
	temp->speed[Y] = 0;

	temp->hp = hp;
	temp->size = size;
	temp->id = ennemyID;
	ennemyID++;	
	
	temp->min[X] = -size/2;
	temp->min[Y] = -size/2;
	temp->max[X] = size/2;
	temp->max[Y] = size/2;	
	
	temp->next = NULL;
	temp->before = NULL;

	return temp;
}
Obstacle createObstacle(float x, float y, int hp, float size){
	Obstacle temp = (Obstacle) malloc( sizeof( struct Entity ) );
	if( !temp){
		printf("Erreur d'allocation\n");
		exit(0);
	}
	temp->pos[X] = x;
	temp->pos[Y] = y;

	temp->speed[X] = 0;
	temp->speed[Y] = 0;

	temp->hp = hp;
	temp->size = size;
	temp->id = obstacleID;
	obstacleID++;
	
	temp->min[X] = -size/2;
	temp->min[Y] = -size/2;
	temp->max[X] = size/2;
	temp->max[Y] = size/2;	
	
	temp->next = NULL;
	temp->before = NULL;

	return temp;
}

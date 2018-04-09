#include "libs.h"

#include "entities.h"

static int ennemyID = 0;
static int obstacleID = 0;
static int bulletID = 0;
static int bonusID = 0;

Ship createShip(float x, float y, int hp, float size){
  Ship ship = (Ship) createEntity(x, y, hp, size, 0);
  ship->damages = SHIP_DAMAGES;
  ship->attackSpeed = SHIP_ATTACK_SPEED;
  return ship;
}
Ennemy createEnnemy(float x, float y, int hp, float size){
	return (Ennemy) createEntity(x, y, hp, size, ennemyID++ );
}
Obstacle createObstacle(float x, float y, int hp, float size){
	return (Obstacle) createEntity(x, y, hp, size, obstacleID++ );
}
Bonus createBonus(float x, float y, int hp, float size, int type) {
  Bonus bonus = (Bonus) createEntity(x, y, hp, size, bonusID++ );
  bonus->damages = 0;
  bonus->bonusType = type;
  return bonus;  
}
Bullet createBullet(Ship ship){
  Bullet bullet = (Bullet) createEntity(ship->pos[X], ship->pos[Y], 1, BULLET_SIZE, bulletID++ );
  
  bullet->speed[X] = BULLET_SPEED;
  bullet->damages = ship->damages;
  
  return bullet;
}

Entity createEntity(float x, float y, int hp, float size, int id) {
  Entity temp = (Entity) malloc( sizeof( struct Entity ) );
	if( !temp){
		printf("Erreur d'allocation\n");
		exit(0);
	}

	temp->hp = hp;
	temp->id = id;
	temp->size = size;
  
	temp->pos[X] = x;
	temp->pos[Y] = y;

	temp->speed[X] = 0;
	temp->speed[Y] = 0;
	
	temp->min[X] = -size/2;
	temp->min[Y] = -size/2;
	temp->max[X] = size/2;
	temp->max[Y] = size/2;
  
  temp->bonusType = NOT_A_BONUS;
  temp->damages = 1;
  temp->attackSpeed = 0;
	
	temp->next = NULL;
	temp->before = NULL;

	return temp;
}

void displayEntity(Entity e) {
  printf("(id:%d) x:%f y:%f | speed: x:%f y:%f | hp:%d | dmg:%d | size:%f\n", e->id, e->pos[X], e->pos[Y], e->speed[X], e->speed[Y], e->hp, e->damages, e->size);
}

/* 
  Return :
  0: Dead
  1: Alive
*/
void getDamage(Entity attacker, Entity defender) {
  defender->hp -= attacker->damages;
}

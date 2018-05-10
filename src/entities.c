#include "libs.h"

#include "entities.h"

static int ennemyID = 0;
static int obstacleID = 0;
static int bulletID = 0;
static int bonusID = 0;

Ship createShip(float x, float y, int hp, float size){
  Ship ship = (Ship) createEntity(x, y, hp, size, 0);
  ship->type = SHIP;
  ship->damages = SHIP_DAMAGES;
  ship->attackPerSecond = SHIP_ATTACK_SPEED;
  ship->missileLevel = 1;
  return ship;
}
Ennemy createEnnemy(float x, float y, int hp, float size){
  Ennemy ennemy = (Ennemy) createEntity(x, y, hp, size, ennemyID++ );
  ennemy->missileLevel = 1;
  ennemy->type = ENNEMY_SIMPLE;
  ennemy->damages = 1;
  ennemy->attackPerSecond = 0.1;
  
  return ennemy;
}
Obstacle createObstacle(float x, float y, int hp, float size){
  Obstacle obstacle = (Obstacle) createEntity(x, y, hp, size, obstacleID++);
  return obstacle;
}
Bonus createBonus(float x, float y, int hp, float size, int type) {
  Bonus bonus = (Bonus) createEntity(x, y, hp, size, bonusID++ );
  bonus->type = type;
  return bonus;  
}
Bullet createBullet(Entity entity, float size){
  Bullet bullet = (Bullet) createEntity(entity->pos[X], entity->pos[Y], 1, size, bulletID++ );

  bullet->damages = entity->damages;

  if ( entity->type != SHIP ) {
    bullet->speed[X] = BULLET_SPEED * 1;
    bullet->speed[X] = -(bullet->speed[X]);
    bullet->pos[X] -= entity->size;
    bullet->type = BULLET_ENNEMY;
  } else {
    bullet->speed[X] = BULLET_SPEED * 1.5;
    bullet->pos[X] += entity->size;
    bullet->type = BULLET_SHIP;
  }
  
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

  temp->damages = 1;
  temp->attackPerSecond = 0;
  temp->missileLevel = 0;
  temp->endOfLevel = 0;
  temp->type = UNDEFINED;
  temp->cooldown = 0;
  
  temp->perlinOffsetX = rand_a_b(0, 256);
  temp->perlinOffsetY = rand_a_b(0, 256);

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

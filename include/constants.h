#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ZOOM 1
#define DIVIDETIME 5
#define SEGMENTSCERCLES 64

#define BULLET_SPEED 0.2
#define BULLET_DAMAGES 10
#define BULLET_SIZE 0.1

#define SHIP_DAMAGES 5
#define SHIP_ATTACK_SPEED 1
#define BOSS_HP 200000.0

#define X 0
#define Y 1
#define ACC 0.035
#define MOMENTUM 0.85
#define SLOW_MULTIPLIER 1.5

#define NOT_A_BONUS -1
#define BONUS_TYPE_DAMAGES 1
#define BONUS_TYPE_ATTACK_SPEED 2
#define BONUS_TYPE_MISSILE_UP 3

#define NOT_AN_ENNEMY -1
#define ENNEMY_TYPE_BASIC 1
#define ENNEMY_TYPE_BOSS 2

#define LEVEL_STATE_INIT 1
#define LEVEL_STATE_RUNNING 2
#define LEVEL_STATE_BOSS_INIT 3
#define LEVEL_STATE_BOSS_SPAWNED 4  
#define LEVEL_STATE_ENDED 5

extern float globalTranslation; /* DEBUG PURPOSE */
extern int LEVEL_STATE;
extern float WINDOW_SCALE;

#endif
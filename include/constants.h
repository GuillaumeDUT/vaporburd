#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ZOOM 1
#define DIVIDETIME 5
#define SEGMENTSCERCLES 64

#define BULLET_SPEED 0.2
#define BULLET_SIZE 0.15

#define SHIP_DAMAGES 5
#define SHIP_ATTACK_SPEED 1
#define BOSS_HP 5000.0

#define X 0
#define Y 1
#define ACC 0.035
#define MOMENTUM 0.85
#define SLOW_MULTIPLIER 1.5

#define LEVEL_STATE_INIT 1
#define LEVEL_STATE_RUNNING 2
#define LEVEL_STATE_BOSS_INIT 3
#define LEVEL_STATE_BOSS_SPAWNED 4  
#define LEVEL_STATE_ENDED 5

#define GAME_MODE_MENU 1
#define GAME_MODE_GAME 2
#define GAME_MODE_END_GAME 3

#define TEXTURE_NUMBER 25
#define PI 3.14159265

/* Couleurs */
#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENTA   "\x1B[35m"
#define CYAN   "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET "\x1B[0m"


extern float globalTranslation; /* DEBUG PURPOSE */
extern int LEVEL_STATE;
extern int GAME_MODE;
extern float WINDOW_SCALE;
extern int DEBUG;

#endif
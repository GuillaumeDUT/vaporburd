#include "libs.h"

#include "obstacle.h"


static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;
static int MUSIC_DURATION = 279000;
float globalTranslation;
int LEVEL_STATE;


/* Affiche et detecte les collisions */
void updateObstacles(Ship ship, OList *obstacles) {
  if(obstacles->taille == 0){
    return ;
  }
  Obstacle oActuel = obstacles->first;
  Obstacle oNext;
  while ( oActuel != NULL ) {
    oNext = oActuel->next;
    drawObstacle( oActuel, 0 );

    /* Collision avec le ship */
    if ( collision(ship, oActuel) ) {
      if ( oActuel->endOfLevel == 0 ) {
        /* Normal obstacle */
        drawShip(ship, 1);
        drawObstacle(oActuel, 1);
        /*
        getDamage(ship, oActuel);
        getDamage(oActuel, ship);
        displayEntity(ship);
        */
      } else if ( oActuel->endOfLevel == 1 && LEVEL_STATE == LEVEL_STATE_RUNNING ) {
        /* End of the level */
        globalTranslation = 100.0 / MUSIC_DURATION * FRAMERATE_MILLISECONDS;

        /* On indique que l'on est arrivé au boss */
        LEVEL_STATE = LEVEL_STATE_BOSS_INIT;
      }
    }

    if ( oActuel->hp <= 0 )  {
      supprimerList(obstacles, oActuel->id);
    }
    oActuel = oNext;
  }
}
void drawObstacle(Obstacle obstacle, int full) {
  glPushMatrix(); {
    if ( obstacle->endOfLevel == 0 ) {
      glColor3f(255, 0, 0);
    } else if ( obstacle->endOfLevel == 1 ) {
      glColor3f(255, 0, 255);    
    }
    glTranslatef(obstacle->pos[X], obstacle->pos[Y], 0);
    glScalef(obstacle->size, obstacle->size, 1);
    drawSquare(full);
  } glPopMatrix();
}

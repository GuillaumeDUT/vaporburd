#include "libs.h"
#include "constants.h"

#include "collisions.h"


/* Detecte les collisions */
int collisionBulletEnnemy(Bullet b, Ennemy e) {
  /* Detect X and Y */
  if ( (b->pos[X]+b->max[X] > e->pos[X]+e->min[X])
      && (b->pos[X]+b->min[X] < e->pos[X]+e->max[X])
      && (b->pos[Y]+b->max[Y] > e->pos[Y]+e->min[Y])
      && (b->pos[Y]+b->min[Y] < e->pos[Y]+e->max[Y])) {
    return 1;
  } else {
    return 0;		
  }		
}

int collisionShipEnnemy(Ship s, Ennemy e) {
  /* Detect X and Y */
  if ( (s->pos[X]+s->max[X] > e->pos[X]+e->min[X])
      && (s->pos[X]+s->min[X] < e->pos[X]+e->max[X])
      && (s->pos[Y]+s->max[Y] > e->pos[Y]+e->min[Y])
      && (s->pos[Y]+s->min[Y] < e->pos[Y]+e->max[Y])) {
    return 1;
  } else {
    return 0;		
  }		
}

/* Detecte les collisions */
int collisionShipObstacle(Ship s, Obstacle o) {
	
	/* Detect X */
	if ( (s->pos[X]+s->max[X] > o->pos[X]+o->min[X])
		&& (s->pos[X]+s->min[X] < o->pos[X]+o->max[X])
		&& (s->pos[Y]+s->max[Y] > o->pos[Y]+o->min[Y])
		&& (s->pos[Y]+s->min[Y] < o->pos[Y]+o->max[Y])) {
		return 1;
	} else {
		return 0;		
	}		
}

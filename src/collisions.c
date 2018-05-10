#include "libs.h"
#include "constants.h"

#include "collisions.h"

int collision(Entity e1, Entity e2) {
  
	if ( (e1->pos[X]+e1->max[X] > e2->pos[X]+e2->min[X])
		&& (e1->pos[X]+e1->min[X] < e2->pos[X]+e2->max[X])
		&& (e1->pos[Y]+e1->max[Y] > e2->pos[Y]+e2->min[Y])
		&& (e1->pos[Y]+e1->min[Y] < e2->pos[Y]+e2->max[Y])) {
		return 1;
	} else {
		return 0;		
	}
}

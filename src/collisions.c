#include "libs.h"
#include "constants.h"

#include "collisions.h"

int collision(Entity e1, Entity e2) {
	// SPHERIC COLLISION
	if ( e1->type == BULLET_ENNEMY ||
			 e2->type == BULLET_ENNEMY ||
			 e1->type == BULLET_BOSS ||
		 	 e2->type == BULLET_BOSS ) {
		// FIX D'UN BUG APPARENT DE COLLISION POUR LES BULLETS DU BOSS
		float deltaX = e1->pos[X] - e2->pos[X];
		float deltaY = e1->pos[Y] - e2->pos[Y];
		float dist = sqrt( pow(deltaX, 2) + pow(deltaY, 2));
		float size = (e1->size + e2->size) / 2;
		
		if ( dist < 0 ) {
			return 0;
		}
		return dist < size ? 1 : 0;
	} 
	// CUBIC COLLISION
	else {
		if ( (e1->pos[X]+e1->max[X] > e2->pos[X]+e2->min[X])
				&& (e1->pos[X]+e1->min[X] < e2->pos[X]+e2->max[X])
				&& (e1->pos[Y]+e1->max[Y] > e2->pos[Y]+e2->min[Y])
				&& (e1->pos[Y]+e1->min[Y] < e2->pos[Y]+e2->max[Y])) {
			return 1;
		} else {
			return 0;		
		}
	}
}

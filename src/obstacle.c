#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "obstacle.h"

void ajouterFinOList(OList *liste, Obstacle obstacle) {

	if ( liste->taille == 0 ) {
		liste->first = obstacle;
		liste->last = obstacle;
	} else {
		liste->last->next = obstacle;
		obstacle->before = liste->last;
		liste->last = obstacle;
	}

	liste->taille++;
}
void afficherOList(OList *liste) {
	Obstacle actuel = liste->first;
  while ( actuel != NULL ) {
    actuel = actuel->next;
  }
}
int supprimerDernierOList(OList *liste) {
  if ( liste->taille == 0 ) {
    printf("Liste vide\n");
    return 0;
  }
  return supprimerOList( liste, liste->last->id);
}
/*
	Supprime une unite de la liste doublement chainée
	L'unite a supprimer est identifiée par son id
*/
int supprimerOList(OList * liste, int id) {
	Obstacle tmp = liste->first;
	int found = 0;
	while ( tmp != NULL && !found ) {
		if ( tmp->id == id ) {
			if ( liste->taille == 1 ) {
				liste->first = NULL;
				liste->last = NULL;
			} else if ( tmp->next == NULL ) {
				liste->last = tmp->before;
				liste->last->next = NULL;
			} else if ( tmp->before == NULL ) {
				liste->first = tmp->next;
				liste->first->before = NULL;
			} else {
				tmp->next->before = tmp->before;
				tmp->before->next = tmp->next;
			}
			found = 1;
			liste->taille--;
		} else {
			tmp = tmp->next;
		}
	}

	return 1;
}


/* Affiche et detecte les collisions */
void loopOList(Ship ship, OList *liste) {
	Obstacle actuel = liste->first;
  while ( actuel != NULL ) {		
		drawObstacle(actuel, 0);
		if ( collisionShipObstacle(ship, actuel) ) {
			drawObstacle(actuel, 1);
		}
    actuel = actuel->next;
  }
}
void drawObstacle(Obstacle obstacle, int full) {	
  glPushMatrix();
		glColor3f(255, 0, 0);
    glTranslatef(obstacle->pos[X], obstacle->pos[Y], 0);
		glScalef(obstacle->size, obstacle->size, 1);
    drawSquare(full);
  glPopMatrix();
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

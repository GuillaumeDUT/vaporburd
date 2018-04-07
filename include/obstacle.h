#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entities.h"

typedef struct OList{
	Obstacle first;
	Obstacle last;
	int taille;
} OList;

void ajouterFinOList(OList *liste , Obstacle obstacle);
void afficherOList(OList *liste);
int supprimerDernierOList(OList *liste);
int supprimerOList(OList * liste, int id);

void loopOList(Ship ship, OList *liste);
void drawObstacle(Obstacle obstacle, int full);
int collisionShipObstacle(Ship s, Obstacle o);

#endif

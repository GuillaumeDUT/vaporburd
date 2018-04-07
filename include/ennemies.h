#ifndef ENNEMIES_H
#define ENNEMIES_H

#include "entities.h"

typedef struct EList{
	Ennemy first;
	Ennemy last;
	int taille;
} EList;

void ajouterFinEList(EList *liste , Ship ship);
void afficherEList(EList *liste);
int supprimerDernierEList(EList *liste);
int supprimerEList(EList * liste, int id);

void loopEList(Ship ship, EList *liste);
void drawEnnemy( Ennemy ennemy );

#endif

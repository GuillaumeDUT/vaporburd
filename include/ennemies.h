#ifndef ENNEMIES_H
#define ENNEMIES_H

#include "entities.h"
#include "shoot.h"
#include "ship.h"

typedef struct EList{
	Ennemy first;
	Ennemy last;
	int taille;
} EList;

void ajouterFinEList(EList *liste , Ship ship);
void afficherEList(EList *liste);
int supprimerDernierEList(EList *liste);
int supprimerEList(EList * liste, int id);
void newRandomEnnemy(EList *ennemiesList, float globalTranslation);

void loopEList(Ship ship, BList *bullets, EList *liste);
void drawEnnemy(Ennemy ennemy, int full);

#endif

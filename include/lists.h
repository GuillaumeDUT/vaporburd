#ifndef LISTS_H
#define LISTS_H

#include "entities.h"

typedef struct List{
	Entity first;
	Entity last;
	int taille;
} List, OList, EList, BList;

void ajouterFinList(List *liste , Entity e);
void afficherList(List *liste);
int supprimerDernierList(List *liste);
int supprimerList(List *liste, int id);
int deleteList(List *liste);

#endif
#ifndef SHOOT_H
#define SHOOT_H

#include "entities.h"


typedef struct Bullet{
	float pos[2];
	float speed;
	int damages;
	int id;
	struct Bullet * next;
	struct Bullet * before;
}*Bullet;

typedef struct BList{
	Bullet first;
	Bullet last;
	int taille;
}BList;

Bullet createBullet(Ship ship);

// tire en fonction de la position actuelle du vaisseau
void shoot(Ship ship, BList *liste);

void ajouterFinBList(BList *liste , Bullet bullet) ;
void afficherBList( BList *liste );
int supprimerDernierBList( BList *liste ) ;
int supprimerBList( BList * liste, int id ) ;
#endif

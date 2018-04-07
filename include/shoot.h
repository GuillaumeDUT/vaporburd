#ifndef SHOOT_H
#define SHOOT_H

#include "constants.h"
#include "entities.h"

typedef struct Bullet{
	float pos[2];
  float min[2];
  float max[2];
	float size;
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
void loopBList(Ship ship, BList *liste);
void drawBullet( Bullet bullet );
void moveBullet( Bullet bullet );

void ajouterFinBList(BList *liste , Bullet bullet) ;
void afficherBList( BList *liste );
int supprimerDernierBList( BList *liste ) ;
int supprimerBList(BList * liste, int id) ;
#endif
